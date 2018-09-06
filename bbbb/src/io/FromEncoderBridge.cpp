#include <Application.h>
#include <Options.h>
#include <io/files/FileIOReceiver.h>
#include <io/FromEncoderBridge.h>
#include <io/network/WebSocketSender.h>
#include <io/TurnAroundStopWatch.h>
#include <string.h>

FromEncoderBridge::FromEncoderBridge()
    : Bridge(new WebSocketSender(Domain::Rotary), new FileIOReceiver("/dev/espi_encoder", 1))
{

  Glib::MainContext::get_default()->signal_timeout().connect(
      [this]() {
        static bool lastSim = true;
        sendRotary(lastSim ? -1 : 1);
        lastSim = !lastSim;
        return true;
      },
      20);
}

FromEncoderBridge::~FromEncoderBridge()
{
}

void FromEncoderBridge::sendRotary(int8_t inc)
{
  if(Application::get().getOptions()->isTurnAroundStopWatchEnabled())
  {
    auto stopWatchPacketID = TurnAroundStopWatch::start();
    int8_t m[5];
    m[0] = inc;
    memcpy(&m[1], &stopWatchPacketID, 4);
    auto msg = Glib::Bytes::create(m, 5);
    m_sender->send(msg);
  }
  else
  {
    auto msg = Glib::Bytes::create(&inc, 1);
    m_sender->send(msg);
  }
}
