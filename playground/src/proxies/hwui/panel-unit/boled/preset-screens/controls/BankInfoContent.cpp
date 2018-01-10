#include <Application.h>
#include <presets/Preset.h>
#include <presets/PresetBank.h>
#include <presets/PresetManager.h>
#include <proxies/hwui/controls/ControlOwner.h>
#include <proxies/hwui/controls/LeftAlignedLabel.h>
#include <proxies/hwui/controls/Rect.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/MultiLineLabel.h>
#include <proxies/hwui/panel-unit/boled/preset-screens/controls/BankInfoContent.h>
#include <iomanip>
#include <sstream>
#include <device-info/DateTimeInfo.h>

namespace DETAIL
{
  class BankComment : public MultiLineLabel
  {
    public:
      BankComment () :
          MultiLineLabel ("---")
      {
      }

      Oleds::tFont getFont ()
      {
        return Oleds::get ().getFont ("Emphase_8_TXT_Regular", 8);
      }
  };
}

static const int divider = 128;

BankInfoContent::BankInfoContent () :
    super (Rect (0, 0, 0, 64))
{
  addControl (m_nameLabel = new LeftAlignedLabel ("Name", Rect (0, 0, divider, 16)));
  addControl (m_commentLabel = new LeftAlignedLabel ("Comment", Rect (0, 16, divider, 16)));
  addControl (m_sizeLabel = new LeftAlignedLabel ("Size", Rect (0, 32, divider, 16)));
  addControl (m_dateOfLastChangeLabel = new LeftAlignedLabel ("Date of Last Change", Rect (0, 48, divider, 16)));
  addControl (m_dateOfImportFileLabel = new LeftAlignedLabel ("Date of Import File", Rect (0, 64, divider, 16)));
  addControl (m_nameOfImportFileLabel = new LeftAlignedLabel ("Name of Import File", Rect (0, 80, divider, 16)));

  m_name = addControl (new DETAIL::BankComment ());
  m_name->setPosition (Rect (divider, 0, 256 - divider, 0));

  m_comment = addControl (new DETAIL::BankComment ());
  m_comment->setPosition (Rect (divider, 0, 256 - divider, 0));

  m_size = addControl (new LeftAlignedLabel ("---", Rect (divider, 32, 256 - divider, 16)));
  m_dateOfLastChange = addControl (new LeftAlignedLabel ("---", Rect (divider, 48, 256 - divider, 16)));
  m_dateOfImportFile = addControl (new LeftAlignedLabel ("---", Rect (divider, 64, 256 - divider, 16)));
  m_nameOfImportFile = addControl (new LeftAlignedLabel ("---", Rect (divider, 80, 256 - divider, 16)));

  Application::get ().getPresetManager ()->onBankSelection (mem_fun (this, &BankInfoContent::onBankSelectionChanged));
}

BankInfoContent::~BankInfoContent ()
{
}

void BankInfoContent::onBankSelectionChanged (shared_ptr<PresetBank> bank)
{
  m_bankConnection.disconnect ();

  if (bank)
    m_bankConnection = bank->onBankChanged (sigc::bind (mem_fun (this, &BankInfoContent::onBankChanged), bank));
}

void BankInfoContent::onBankChanged (shared_ptr<PresetBank> bank)
{
  if (bank)
  {
    if(fillFromBank (bank.get ()))
    {
      fixLayout ();
    }
  }
  else if (fillDefaults ())
  {
    fixLayout ();
  }
}

bool BankInfoContent::fillFromBank (const PresetBank *bank)
{
  bool ret = m_name->setText (bank->getName (true), FrameBuffer::Colors::C128);
  ret |= m_comment->setText (bank->getAttribute ("Comment", "---"), FrameBuffer::Colors::C128);
  ret |= m_size->setText (to_string (bank->getNumPresets ()));
  ret |= m_dateOfLastChange->setText (DateTimeInfo::formatTime (bank->getLastChangedTimestamp (), "%x %X"));
  ret |= m_dateOfImportFile->setText (DateTimeInfo::formatTime (bank->getAttribute ("Date of Import File", "---"), "%x %X"));
  ret |= m_nameOfImportFile->setText (bank->getAttribute ("Name of Import File", "---"));
  return ret;
}

bool BankInfoContent::fillDefaults ()
{
  bool ret = m_name->setText ("---", FrameBuffer::Colors::C128);
  ret |= m_comment->setText ("---", FrameBuffer::Colors::C128);
  ret |= m_size->setText ("---");
  ret |= m_dateOfLastChange->setText ("---");
  ret |= m_dateOfImportFile->setText ("---");
  ret |= m_nameOfImportFile->setText ("---");
  return ret;
}

void BankInfoContent::setPosition (const Rect &rect)
{
  super::setPosition (rect);
  fixLayout ();
}

const Rect &BankInfoContent::getPosition () const
{
  return super::getPosition ();
}

void BankInfoContent::setDirty ()
{
  super::setDirty ();
  notifyDirty (true);
}

void BankInfoContent::fixLayout ()
{
  int y = 0;
  m_nameLabel->setPosition (Rect (0, y, divider, 16));
  m_name->setPosition (Rect (divider, y + 2, 256 - divider, m_name->getPosition ().getHeight ()));
  y = std::max (m_name->getPosition ().getBottom (), m_nameLabel->getPosition ().getBottom ());

  m_commentLabel->setPosition (Rect (0, y, divider, 16));
  m_comment->setPosition (Rect (divider, y + 2, 256 - divider, m_comment->getPosition ().getHeight ()));
  y = std::max (m_commentLabel->getPosition ().getBottom (), m_comment->getPosition ().getBottom ());

  m_sizeLabel->setPosition (Rect (0, y, divider, 16));
  m_size->setPosition (Rect (divider, y, 256 - divider, 16));
  y += 16;

  m_dateOfLastChangeLabel->setPosition (Rect (0, y, divider, 16));
  m_dateOfLastChange->setPosition (Rect (divider, y, 256 - divider, 16));
  y += 16;

  m_dateOfImportFileLabel->setPosition (Rect (0, y, divider, 16));
  m_dateOfImportFile->setPosition (Rect (divider, y, 256 - divider, 16));
  y += 16;

  m_nameOfImportFileLabel->setPosition (Rect (0, y, divider, 16));
  m_nameOfImportFile->setPosition (Rect (divider, y, 256 - divider, 16));
  y += 16;

  Rect r = getPosition ();
  r.setHeight (y);
  super::setPosition (r);
}
