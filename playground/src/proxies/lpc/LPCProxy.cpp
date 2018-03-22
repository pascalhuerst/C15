#include "LPCProxy.h"
#include "Application.h"
#include "presets/PresetManager.h"
#include "presets/EditBuffer.h"
#include "parameters/Parameter.h"
#include "parameters/ModulateableParameter.h"
#include "parameters/PhysicalControlParameter.h"
#include <parameters/RibbonParameter.h>
#include "MessageParser.h"
#include "ParameterMessageComposer.h"
#include "EditBufferMessageComposer.h"
#include "libundo/undo/Scope.h"
#include "http/UndoScope.h"
#include "proxies/hwui/Layout.h"
#include "device-settings/SendPresetAsLPCWriteFallback.h"
#include "device-settings/Settings.h"
#include "proxies/hwui/HWUI.h"
#include "groups/HardwareSourcesGroup.h"
#include "device-settings/DebugLevel.h"
#include "device-settings/VelocityCurve.h"
#include <device-settings/ParameterEditModeRibbonBehaviour.h>
#include <memory.h>

const char *s_fileName = "/dev/lpc_bb_driver";

const int c_testTimePerFrequency = 5000;

LPCProxy::LPCProxy () :
    m_ioCancel (Gio::Cancellable::create ()),
    m_queueSendingScheduled (false),
    m_lpcPollerInstalled (false),
    m_lastTouchedRibbon (HardwareSourcesGroup::getUpperRibbonParameterID ()),
    m_throttledRelativeParameterChange (std::chrono::milliseconds (1)),
    m_throttledAbsoluteParameterChange (std::chrono::milliseconds (1))
{
  open ();
}

connection LPCProxy::onRibbonTouched (slot<void, int> s)
{
  return m_signalRibbonTouched.connectAndInit (s, m_lastTouchedRibbon);
}

connection LPCProxy::onLPCSoftwareVersionChanged (slot<void, int> s)
{
  return m_signalLPCSoftwareVersionChanged.connectAndInit (s, m_lpcSoftwareVersion);
}

int LPCProxy::getLastTouchedRibbonParameterID () const
{
  return m_lastTouchedRibbon;
}

LPCProxy::~LPCProxy ()
{
  DebugLevel::warning (__PRETTY_FUNCTION__, __LINE__);
  m_ioCancel->cancel ();
  DebugLevel::warning (__PRETTY_FUNCTION__, __LINE__);
}

void LPCProxy::open ()
{
  DebugLevel::gassy ("Open lpc drivers file");

#ifdef _DEVELOPMENT_PC

  RefPtr<Gio::File> ioFile = Gio::File::create_for_path ("/tmp/lpc-dump");

  if (!ioFile->query_exists ())
  {
    ioFile->create_file (Gio::FILE_CREATE_REPLACE_DESTINATION);
  }

  ioFile->append_to_async (sigc::bind (sigc::mem_fun (this, &LPCProxy::onOutputStreamOpened), ioFile), m_ioCancel);

#else

  RefPtr < Gio::File > ioFile = Gio::File::create_for_path (s_fileName);

  if (ioFile->query_exists ())
  {
    ioFile->read_async (sigc::bind (sigc::mem_fun (this, &LPCProxy::onInputStreamOpened), ioFile), m_ioCancel);
    ioFile->append_to_async (sigc::bind (sigc::mem_fun (this, &LPCProxy::onOutputStreamOpened), ioFile), m_ioCancel);
  }
  else
  {
    DebugLevel::warning ("Could not open lpc_bb_driver device file!");
  }

#endif
}

void LPCProxy::onInputStreamOpened (Glib::RefPtr<Gio::AsyncResult> &result, RefPtr<Gio::File> ioFile)
{
  try
  {
    DebugLevel::gassy ("LPCProxy::onInputStreamOpened file");
    m_inStream = Gio::BufferedInputStream::create (ioFile->read_finish (result));
    readIO (m_inStream, MessageParser::getNumInitialBytesNeeded ());
  }
  catch (Gio::Error &error)
  {
    DebugLevel::warning ("Could not read from device file stream");
  }
}

void LPCProxy::onOutputStreamOpened (Glib::RefPtr<Gio::AsyncResult> &result, RefPtr<Gio::File> ioFile)
{
  try
  {
    DebugLevel::gassy ("LPCProxy::onOutputStreamOpened file");
    m_outStream = ioFile->append_to_finish (result);
  }
  catch (Gio::Error &error)
  {
    DebugLevel::warning ("Could not write to device file stream");
  }
}

void LPCProxy::readIO (Glib::RefPtr<Gio::InputStream> stream, size_t numBytes)
{
  if (Glib::RefPtr<Gio::BufferedInputStream>::cast_dynamic (stream)->get_available () >= numBytes)
  {
    auto bytes = stream->read_bytes (numBytes, m_ioCancel);
    onLPCDataRead (stream, bytes);
  }
  else
  {
    stream->read_bytes_async (numBytes, sigc::bind (sigc::mem_fun (this, &LPCProxy::onIOFileRead), stream), m_ioCancel,
        Glib::PRIORITY_HIGH);
  }
}

void LPCProxy::onIOFileRead (Glib::RefPtr<Gio::AsyncResult> &result, Glib::RefPtr<Gio::InputStream> stream)
{
  Glib::RefPtr<Glib::Bytes> bytes = stream->read_bytes_finish (result);
  onLPCDataRead (stream, bytes);
}

void LPCProxy::onLPCDataRead (Glib::RefPtr<Gio::InputStream> stream, Glib::RefPtr<Glib::Bytes> bytes)
{
  gsize numBytes = 0;
  const uint8_t* buffer = (const uint8_t*) (bytes->get_data (numBytes));

  if (numBytes > 0)
  {
    if (!m_msgParser)
      m_msgParser.reset (new MessageParser ());

    size_t numBytesNeeded = m_msgParser->parse (buffer, numBytes);

    if (numBytesNeeded == 0)
    {
      onMessageReceived (std::move (m_msgParser->getMessage ()));
      m_msgParser.reset ();
      numBytesNeeded = MessageParser::getNumInitialBytesNeeded ();
    }

    readIO (stream, numBytesNeeded);
  }
  else
  {
    DebugLevel::warning ("read 0 bytes from lpc - reopen device driver file!");
    open ();
  }
}

gint16 LPCProxy::separateSignedBitToComplementary (uint16_t v) const
{
  gint16 sign = (v & 0x8000) ? -1 : 1;
  gint16 value = (v & 0x7FFF);
  return sign * value;
}

void LPCProxy::onMessageReceived (const MessageParser::NLMessage &msg)
{
  if (msg.type == MessageParser::PARAM)
  {
    onParamMessageReceived (msg);
  }
  else if (msg.type == MessageParser::EDIT_CONTROL)
  {
    onEditControlMessageReceived (msg);
  }
  else if (msg.type == MessageParser::ASSERTION)
  {
    onAssertionMessageReceived (msg);
  }
  else if (msg.type == MessageParser::NOTIFICATION)
  {
    onNotificationMessageReceived (msg);
  }
}

void LPCProxy::onAssertionMessageReceived (const MessageParser::NLMessage &msg)
{
  auto numWords = msg.length;
  auto numBytes = numWords * 2;
  const char *bytes = (const char *) msg.params.data ();
  char txt[numBytes + 2];
  strncpy (txt, bytes, numBytes);
  txt[numBytes] = '\0';
  DebugLevel::error ("!!! Assertion from LPC in", (const char *) txt);
}

void LPCProxy::onNotificationMessageReceived (const MessageParser::NLMessage &msg)
{
  uint16_t id = msg.params[0];
  uint16_t value = msg.params[1];

  if (id == MessageParser::SOFTWARE_VERSION)
  {
    if (m_lpcSoftwareVersion != value)
    {
      m_lpcSoftwareVersion = value;
      m_signalLPCSoftwareVersionChanged.send (m_lpcSoftwareVersion);
    }
  }
}

void LPCProxy::onParamMessageReceived (const MessageParser::NLMessage &msg)
{
  DebugLevel::info ("it is a param message");

  uint16_t id = msg.params[0];
  notifyRibbonTouch (id);

  gint16 value = separateSignedBitToComplementary (msg.params[1]);
  auto param = Application::get ().getPresetManager ()->getEditBuffer ()->findParameterByID (id);

  if (auto p = dynamic_cast<PhysicalControlParameter *> (param))
  {
    DebugLevel::info (G_STRLOC, value);
    applyParamMessageAbsolutely (p, value);
  }
}

void LPCProxy::applyParamMessageAbsolutely (PhysicalControlParameter *p, gint16 value)
{
  DebugLevel::info (G_STRLOC, value);

  if (p->isBiPolar ())
  {
    DebugLevel::info (G_STRLOC);
    p->onChangeFromLpc ((value - 8000.0) / 8000.0);
  }
  else
  {
    DebugLevel::info (G_STRLOC);
    p->onChangeFromLpc (value / 16000.0);
  }
}

void LPCProxy::onEditControlMessageReceived (const MessageParser::NLMessage &msg)
{
  DebugLevel::info ("it is an edit control message");

  uint16_t id = msg.params[0];
  notifyRibbonTouch (id);

  gint16 value = separateSignedBitToComplementary (msg.params[1]);

  if (auto p = Application::get ().getPresetManager ()->getEditBuffer ()->getSelected ())
  {
    auto ribbonModeBehaviour = Application::get ().getSettings ()->getSetting<ParameterEditModeRibbonBehaviour> ()->get ();

    if (ribbonModeBehaviour == ParameterEditModeRibbonBehaviours::PARAMETER_EDIT_MODE_RIBBON_BEHAVIOUR_RELATIVE)
    {
      DebugLevel::info (G_STRLOC, value);
      onRelativeEditControlMessageReceived (p, value);
    }
    else
    {
      DebugLevel::info (G_STRLOC, value);
      onAbsoluteEditControlMessageReceived (p, value);
    }
  }
}

void LPCProxy::onRelativeEditControlMessageReceived (Parameter *p, gint16 value)
{
  m_throttledRelativeParameterAccumulator += value;

  m_throttledRelativeParameterChange.doTask ([this, p]()
  {
    if (!m_relativeEditControlMessageChanger || !m_relativeEditControlMessageChanger->isManaging (p->getValue ()))
    m_relativeEditControlMessageChanger = p->getValue ().startUserEdit (Initiator::EXPLICIT_LPC);

    m_relativeEditControlMessageChanger->changeBy (m_throttledRelativeParameterAccumulator / (p->isBiPolar () ? 8000.0 : 16000.0));
    m_throttledRelativeParameterAccumulator = 0;
  });
}

void LPCProxy::onAbsoluteEditControlMessageReceived (Parameter *p, gint16 value)
{
  m_throttledAbsoluteParameterValue = value;

  m_throttledAbsoluteParameterChange.doTask ([this, p]()
  {
    auto scope = Application::get ().getUndoScope ()->startContinuousTransaction (p, "Set '%0'", p->getGroupAndParameterName ());

    if (p->isBiPolar ())
    {
      p->setCPFromHwui (scope->getTransaction (), (m_throttledAbsoluteParameterValue - 8000.0) / 8000.0);
      DebugLevel::info ("set it (absolutely - bipolar) to", p->getControlPositionValue ());
    }
    else
    {
      p->setCPFromHwui (scope->getTransaction (), (m_throttledAbsoluteParameterValue / 16000.0));
      DebugLevel::info ("set it (absolutely - unipolar) to", p->getControlPositionValue ());
    }
  });
}

void LPCProxy::notifyRibbonTouch (int ribbonsParameterID)
{
  if (ribbonsParameterID == HardwareSourcesGroup::getLowerRibbonParameterID () || ribbonsParameterID
      == HardwareSourcesGroup::getUpperRibbonParameterID ())
  {
    m_lastTouchedRibbon = ribbonsParameterID;
    m_signalRibbonTouched.send (ribbonsParameterID);
  }
}

void LPCProxy::sendParameter (const Parameter *param)
{
  if (!m_suppressParamChanges)
  {
    tMessageComposerPtr cmp (new ParameterMessageComposer (param));
    queueToLPC (cmp);
  }
}

void LPCProxy::queueToLPC (tMessageComposerPtr cmp)
{
  if (!m_queueToLPC.empty ())
    if (cmp->canReplace (m_queueToLPC.back ().get ()))
      m_queueToLPC.pop_back ();

  m_queueToLPC.push_back (cmp);

  bool expected = false;

  if (m_queueSendingScheduled.compare_exchange_strong (expected, true))
    Application::get ().getMainContext ()->signal_idle ().connect (sigc::mem_fun (this, &LPCProxy::sendQueue));
}

bool LPCProxy::sendQueue ()
{
  m_queueSendingScheduled = false;
  writePendingData ();
  return false;
}

void LPCProxy::writePendingData ()
{
  if (!m_queueToLPC.empty () && m_outStream && !m_writingData)
  {
    DebugLevel::info ("write pending data\n");

    tMessageComposerPtr cmp = m_queueToLPC.front ();
    m_queueToLPC.pop_front ();

    auto flushed = cmp->flush ();

    try
    {
      traceBytes (flushed);
      m_writingData = true;
      m_outStream->write_bytes_async (flushed, sigc::mem_fun (this, &LPCProxy::wroteData), m_ioCancel);
    }
    catch (Gio::Error error)
    {
      DebugLevel::warning ("write_async threw Gio::Error =>", error.what ());
      installLPCPoller ();
    }
    catch (...)
    {
      DebugLevel::warning ("write_async threw unknown error");
      installLPCPoller ();
    }
  }
}

void LPCProxy::traceBytes (const RefPtr<Bytes> bytes) const
{
  if (Application::get ().getSettings ()->getSetting<DebugLevel> ()->getLevel () == DebugLevels::DEBUG_LEVEL_GASSY)
  {
    gsize numBytes = 0;
    uint8_t *data = (uint8_t *) bytes->get_data (numBytes);

    char txt[numBytes * 4];
    char *ptr = txt;

    for (size_t i = 0; i < numBytes; i++)
      ptr += sprintf (ptr, "%02x ", data[i]);

    *ptr = '\0';
    DebugLevel::gassy ((const char *) txt);
  }
}

void LPCProxy::wroteData (Glib::RefPtr<Gio::AsyncResult> &result)
{
  DebugLevel::info ("wrote pending data");

  try
  {
    m_outStream->write_bytes_finish (result);
  }
  catch (Gio::Error error)
  {
    DebugLevel::warning ("wroteData threw Gio::Error =>", error.what ());
    installLPCPoller ();
  }
  catch (...)
  {
    DebugLevel::warning ("wroteData threw unknown error");
    installLPCPoller ();
  }

  m_writingData = false;
  writePendingData ();
}

bool LPCProxy::isFlushed() const
{
  if(m_queueToLPC.empty())
  {
    return !m_outStream->has_pending();
  }
  return false;
}

void LPCProxy::installLPCPoller ()
{
  if (Application::get ().getSettings ()->getSetting<SendPresetAsLPCWriteFallback> ()->get ())
  {
    if (!m_lpcPollerInstalled)
    {
      m_lpcPollerInstalled = true;

      DebugLevel::warning ("installLPCPoller");

      m_queueToLPC.clear ();
      Application::get ().getMainContext ()->signal_timeout ().connect_once (sigc::mem_fun (this, &LPCProxy::pollLPC), 1000);
    }
  }
}

void LPCProxy::pollLPC ()
{
  m_lpcPollerInstalled = false;
  DebugLevel::warning ("pollLPC - sendPreset");
  sendEditBuffer ();
}

void LPCProxy::sendEditBuffer ()
{
  DebugLevel::info ("send preset to LPC");

  auto editBuffer = Application::get ().getPresetManager ()->getEditBuffer ();

  tMessageComposerPtr cmp (new EditBufferMessageComposer ());

  auto sorted = editBuffer->getParametersSortedById ();

  for (auto & it : sorted)
    it.second->writeToLPC (*cmp);

  queueToLPC (cmp);

  editBuffer->getSettings ().sendToLPC ();
  Application::get ().getSettings ()->sendToLPC ();

  for (auto & it : sorted)
    it.second->onPresetSentToLpc ();
}

void LPCProxy::toggleSuppressParameterChanges (UNDO::Scope::tTransactionPtr transaction)
{
  transaction->addSimpleCommand ([ = ] (UNDO::Command::State) mutable
  {
    m_suppressParamChanges = !m_suppressParamChanges;

    if (!m_suppressParamChanges)
    {
      sendEditBuffer ();
    }
  });
}

void LPCProxy::sendSetting (uint16_t key, uint16_t value)
{
  tMessageComposerPtr cmp (new MessageComposer (MessageParser::SETTING));
  *cmp << key;
  *cmp << value;
  queueToLPC (cmp);

  DebugLevel::info ("sending setting", key, "=", value);
}

void LPCProxy::sendSetting (uint16_t key, gint16 value)
{
  tMessageComposerPtr cmp (new MessageComposer (MessageParser::SETTING));
  *cmp << key;
  *cmp << value;
  queueToLPC (cmp);

  DebugLevel::info ("sending setting", key, "=", value);
}

void LPCProxy::sendSetting (uint16_t key, bool value)
{
  sendSetting (key, (uint16_t) (value ? 1 : 0));
}

void LPCProxy::requestLPCSoftwareVersion ()
{
  tMessageComposerPtr cmp (new MessageComposer (MessageParser::REQUEST));
  uint16_t v = MessageParser::SOFTWARE_VERSION;
  *cmp << v;
  queueToLPC (cmp);

  DebugLevel::info ("sending request", MessageParser::SOFTWARE_VERSION);
}

int LPCProxy::getLPCSoftwareVersion () const
{
  return m_lpcSoftwareVersion;
}
