#include "Preset.h"
#include "PresetBank.h"
#include "PresetManager.h"
#include "EditBuffer.h"
#include "xml/Writer.h"
#include "libundo/undo/SwapData.h"
#include "http/UpdateDocumentMaster.h"
#include "Application.h"
#include "xml/FileInStream.h"
#include <xml/VersionAttribute.h>
#include <xml/CommitableFileOutStream.h>
#include <serialization/PresetSerializer.h>
#include <tools/PerformanceTimer.h>
#include <libundo/undo/TrashTransaction.h>
#include <device-settings/DebugLevel.h>
#include <device-settings/DeviceName.h>
#include <device-info/DateTimeInfo.h>
#include <device-settings/Settings.h>
#include <device-info/DeviceInformation.h>
#include "SearchQuery.h"
#include <testing/TestDriver.h>

shared_ptr<Preset> Preset::createPreset (UpdateDocumentContributor *parent)
{
  shared_ptr<Preset> p (new Preset (parent));
  p->init ();
  return p;
}

Preset::Preset (UpdateDocumentContributor *parent) :
    ParameterGroupSet (parent),
    m_settings (*this)
{
}

Preset::~Preset ()
{
}

void Preset::setAutoGeneratedAttributes ()
{
  setAttribute ("DeviceName", Application::get ().getSettings ()->getSetting<DeviceName> ()->get ());
  setAttribute ("StoreTime", Application::get ().getDeviceInformation ()->getItem<DateTimeInfo> ()->get ());
  setAttribute ("SoftwareVersion", Application::get ().getDeviceInformation ()->getSoftwareVersion ());

  updateBankChangeTime();
}

void Preset::updateBankChangeTime()
{
  getBank()->onChange();
}

void Preset::load (UNDO::Scope::tTransactionPtr transaction, RefPtr<Gio::File> presetPath)
{
  Glib::ustring strUUID = getUuid ();
  Serializer::read<PresetSerializer> (transaction, presetPath, strUUID, this);

  m_lastSavedUpdateID = getUpdateIDOfLastChange ();
}

bool Preset::save (RefPtr<Gio::File> bankPath)
{
  if (m_lastSavedUpdateID != getUpdateIDOfLastChange ())
  {
    PresetSerializer serializer (this);
    Glib::ustring strUUID = getUuid ();
    serializer.write (bankPath, strUUID);
    m_lastSavedUpdateID = getUpdateIDOfLastChange ();
    return true;
  }
  return false;
}

shared_ptr<const PresetBank> Preset::getBank () const
{
  return static_cast<const PresetBank *> (getParent ())->shared_from_this ();
}

shared_ptr<PresetBank> Preset::getBank ()
{
  return static_cast<PresetBank *> (getParent ())->shared_from_this ();
}

void Preset::setName (const ustring &name)
{
  m_name = name;
}

void Preset::undoableSetName (const ustring &name)
{
  if (name != m_name)
  {
    UNDO::Scope::tTransactionScopePtr scope = getUndoScope ().startTransaction ("Rename preset '%0' to '%1'", getName (), name);
    undoableSetName (scope->getTransaction (), name);
  }
}

void Preset::undoableSetName (UNDO::Scope::tTransactionPtr transaction, const ustring &name)
{
  if (name != m_name)
  {
    auto swapData = UNDO::createSwapData (name);

    transaction->addSimpleCommand ([ = ] (UNDO::Command::State) mutable
    {
      swapData->swapWith (m_name);
      onChange ();
    });

    Application::get ().getPresetManager ()->getEditBuffer ()->undoableUpdateLoadedPresetInfo (transaction);
  }
}

void Preset::undoableSetType (UNDO::Scope::tTransactionPtr transaction, PresetType desiredType)
{
  if (m_type != desiredType)
  {
    for (auto g : getParameterGroups ())
    {
      g->undoableSetType (transaction, m_type, desiredType);
    }

    auto swapData = UNDO::createSwapData (desiredType);

    transaction->addSimpleCommand ([ = ] (UNDO::Command::State) mutable
    {
      swapData->swapWith (m_type);
    });
  }
}

void Preset::undoableSetUuid (UNDO::Scope::tTransactionPtr transaction, const Uuid &uuid)
{
  setUuid (transaction, uuid);
}

UpdateDocumentContributor::tUpdateID Preset::onChange ()
{
  if (getParent ())
  {
    auto u = ParameterGroupSet::onChange ();
    m_signalPresetChanged.send ();
    return u;
  }

  return 0;
}

ustring Preset::getName () const
{
  return m_name;
}

const Uuid &Preset::getUuid () const
{
  return m_uuid;
}

void Preset::setUuid (UNDO::Scope::tTransactionPtr transaction, const Glib::ustring &id)
{
  Uuid uuid (id);
  auto swapData = UNDO::createSwapData (uuid);

  transaction->addSimpleCommand ([ = ] (UNDO::Command::State) mutable
  {
    swapData->swapWith (m_uuid);
    onChange();
  });
}

sigc::connection Preset::onPresetChanged (slot<void> s)
{
  return m_signalPresetChanged.connectAndInit (s);
}

void Preset::writeDocument (Writer &writer, tUpdateID knownRevision) const
{
  bool changed = knownRevision < getUpdateIDOfLastChange ();

  writer.writeTag ("preset", Attribute ("uuid", (Glib::ustring) m_uuid), Attribute ("name", m_name), Attribute ("changed", changed), [&]()
  {
    if (changed)
    {
      writeAttributes(writer);
      m_settings.writeDocument (writer, knownRevision);
    }
  });
}

void Preset::copyFrom (UNDO::Scope::tTransactionPtr transaction, Preset *other, bool ignoreUUIDs)
{
  ParameterGroupSet::copyFrom (transaction, other);

  if (auto preset = other)
  {
    m_settings.copyFrom (transaction, preset->getSettings ());
    AttributesOwner::copyFrom(transaction, preset);

    auto swapData = UNDO::createSwapData (preset->m_name);

    transaction->addSimpleCommand ([ = ] (UNDO::Command::State) mutable
    {
      swapData->swapWith (m_name);
      onChange ();
    });

    if (!ignoreUUIDs)
      undoableSetUuid (transaction, other->getUuid ());
  }
}

void Preset::undoableSelect (UNDO::Scope::tTransactionPtr transaction)
{
  getBank ()->undoableSelectPreset (transaction, getUuid ());
}

void Preset::guessName (UNDO::Scope::tTransactionPtr transaction)
{
  Glib::ustring currentName = getName ();

  if (currentName.empty ())
    currentName = "New preset";

  undoableSetName (transaction, "");
  Glib::ustring newName = getBank ()->getParent ()->createPresetNameBasedOn (currentName);
  undoableSetName (transaction, newName);
}

PresetSettings &Preset::getSettings ()
{
  return m_settings;
}

const PresetSettings &Preset::getSettings () const
{
  return m_settings;
}

size_t Preset::getHash () const
{
  size_t hash = AttributesOwner::getHash();
  hash_combine (hash, m_settings.getHash ());

  for (auto g : getParameterGroups ())
  {
    hash_combine (hash, g->getHash ());
  }

  return hash;
}

bool Preset::isLoaded () const
{
  auto a = Application::get ().getPresetManager ()->getEditBuffer ()->getUUIDOfLastLoadedPreset ();
  Glib::ustring b = getUuid ();
  return a == b;
}

bool Preset::hasDuplicateName () const
{
  size_t num = 0;

  for (auto bank : Application::get ().getPresetManager ()->getBanks ())
  {
    for (auto preset : bank->getPresets ())
    {
      if (preset->getName () == getName ())
      {
        num++;

        if (num > 1)
        {
          return true;
        }
      }
    }
  }

  return false;
}

Glib::ustring Preset::getUndoTransactionTitle (const Glib::ustring &prefix) const
{
  if (auto bank = getBank ())
  {
    int bankNumber = bank->getParent ()->calcOrderNumber (bank.get ());
    int presetNumber = bank->getPresetPosition (getUuid ()) + 1;
    char txt[256];
    sprintf (txt, "%d-%03d", bankNumber, presetNumber);
    return UNDO::StringTools::buildString (prefix, " ", txt, ": '", getName (), "'");
  }

  return UNDO::StringTools::buildString (prefix, " '", getName (), "'");
}

bool Preset::matchesQuery (const SearchQuery &query) const
{
  return query.iterate ([&](const Glib::ustring &part)
  {
    return doesQuerySubstringMatch(part);
  });
}

bool Preset::doesQuerySubstringMatch (const Glib::ustring &part) const
{
  if (m_name.lowercase ().find (part) != Glib::ustring::npos)
    return true;

  if(AttributesOwner::doesAnyAttributeMatch(part))
    return true;

  if (m_settings.matchesQuery (part))
    return true;

  return false;
}

void Preset::onLoadStatusChanged()
{
  #warning "testweise auskommentiert - ich halte das fuer obsolet und kontra-produktiv (10.1.2018)"
  #warning "bitte ausbauen, wenn keine Probleme in diesem Scope auftauchen. Wenn onChange hier"
  #warning "benoetigt wird, bitte expliziter formulieren, so dass man versteht, warum!?"
  // onChange();
}
