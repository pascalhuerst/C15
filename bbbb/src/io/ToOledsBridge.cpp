#include <Application.h>
#include <Options.h>
#include <io/framebuffer/FrameBufferSender.h>
#include <io/network/WebSocketReceiver.h>
#include <io/ToOledsBridge.h>
#include <io/TurnAroundStopWatch.h>

ToOledsBridge::ToOledsBridge()
    : super(new FrameBufferSender(), new WebSocketReceiver(Domain::Oled))
{
}

ToOledsBridge::~ToOledsBridge()
{
}

void ToOledsBridge::transmit(Receiver::tMessage msg)
{
  if(Application::get().getOptions()->isTurnAroundStopWatchEnabled())
  {
    gsize l = 0;
    uint32_t *raw = (uint32_t *) msg->get_data(l);
    uint32_t numPacketIDs = raw[0];
    TurnAroundStopWatch::stop(&raw[1], numPacketIDs);

    auto baseMsg = Glib::Bytes::create(&raw[numPacketIDs + 1], l - 4 * (numPacketIDs + 1));
    Bridge::transmit(baseMsg);
  }
  else
  {
    Bridge::transmit(msg);
  }
}
