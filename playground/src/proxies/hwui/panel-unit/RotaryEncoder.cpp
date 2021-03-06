#include <Application.h>
#include <Options.h>
#include <device-settings/DebugLevel.h>
#include <device-settings/EncoderAcceleration.h>
#include <device-settings/Settings.h>
#include <proxies/hwui/panel-unit/RotaryEncoder.h>
#include <testing/TestDriver.h>
#include <proxies/lpc/LPCProxy.h>
#include <tools/TimeTools.h>
#include <string.h>

static TestDriver<RotaryEncoder> tester;

RotaryEncoder::RotaryEncoder()
    : m_throttler(chrono::milliseconds(10))
{
  Application::get().getWebSocketSession()->onMessageReceived(WebSocketSession::Domain::Rotary,
                                                              sigc::mem_fun(this, &RotaryEncoder::onMessage));
}

RotaryEncoder::~RotaryEncoder()
{
  m_stress.disconnect();
}

void RotaryEncoder::onMessage(const WebSocketSession::tMessages &msgs)
{
  int summedInc = 0;

  for(auto &msg : msgs)
  {
    gsize numBytes = 0;
    const char *buffer = (const char *) msg->get_data(numBytes);

    if(numBytes > 0)
      summedInc += buffer[0];

    if(Application::get().getOptions()->isTurnAroundStopWatchEnabled())
    {
      using namespace std::chrono;

      uint32_t id = 0;
      memcpy(&id, &buffer[1], 4);
      receivedMessageIDs.push_back(id);
      std::cout << "Rotary proxy received packet " << id << " at "
                << duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch()).count() << std::endl;
    }
  }

  if(summedInc)
    applyIncrement(summedInc);
}

void RotaryEncoder::applyIncrement(tIncrement currentInc)
{
  m_signalRotaryChanged.send(currentInc);

  m_accumulatedIncs += currentInc;

  if((currentInc < 0) && (m_accumulatedIncs > 0))
    m_accumulatedIncs = 0;

  if((currentInc > 0) && (m_accumulatedIncs < 0))
    m_accumulatedIncs = 0;

  m_throttler.doTask([this]() {
    if(abs(m_accumulatedIncs) > 1)
    {
      m_accumulatedIncs = std::min(m_accumulatedIncs, 10);
      m_accumulatedIncs = std::max(m_accumulatedIncs, -10);
      double factor = Application::get().getSettings()->getSetting<EncoderAcceleration>()->get();
      int sign = m_accumulatedIncs < 0 ? -1 : 1;
      m_signalRotaryChanged.send(factor * sign * m_accumulatedIncs * m_accumulatedIncs);
    }
    m_accumulatedIncs = 0;
  });
}

void RotaryEncoder::fake(tIncrement i)
{
  m_signalRotaryChanged.send(i);
}

sigc::connection RotaryEncoder::onRotaryChanged(function<void(tIncrement)> slot)
{
  return m_signalRotaryChanged.connect(slot);
}

void RotaryEncoder::registerTests()
{
}
