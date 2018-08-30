#include <io/framebuffer/FrameBufferSender.h>
#include <io/network/WebSocketReceiver.h>
#include <io/ToOledsBridge.h>
#include <io/TurnAroundStopWatch.h>

ToOledsBridge::ToOledsBridge() :
    super(new FrameBufferSender(), new WebSocketReceiver(Domain::Oled))
{
}

ToOledsBridge::~ToOledsBridge()
{
}

void ToOledsBridge::transmit(Receiver::tMessage msg)
{
  gsize l = 0;
  uint32_t *raw = (uint32_t*)msg->get_data(l);
  uint32_t numPacketIDs = raw[0];

  for(uint32_t packetID = 0; packetID < numPacketIDs; packetID++)
  {
    TurnAroundStopWatch::stopAndRemove(raw[packetID + 1]);
  }

  auto baseMsg = Glib::Bytes::create(&raw[numPacketIDs + 1], l - 4 * (numPacketIDs + 1));
  Bridge::transmit(baseMsg);
}
