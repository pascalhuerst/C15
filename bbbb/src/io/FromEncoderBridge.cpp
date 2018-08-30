#include <io/files/FileIOReceiver.h>
#include <io/FromEncoderBridge.h>
#include <io/network/WebSocketSender.h>
#include <io/TurnAroundStopWatch.h>

FromEncoderBridge::FromEncoderBridge()
    : Bridge(new WebSocketSender(Domain::Rotary), new FileIOReceiver("/dev/espi_encoder", 1))
{
}

FromEncoderBridge::~FromEncoderBridge()
{
}

void FromEncoderBridge::sendRotary(int8_t inc)
{
  auto stopWatchPacketID = TurnAroundStopWatch::start();
  int8_t m[5];
  m[0] = inc;
  memcpy(&m[1], &stopWatchPacketID, 4);
  auto msg = Glib::Bytes::create(m, 5);
  m_sender->send(msg);

  std::cerr << "sent encoder event " << stopWatchPacketID << " at " << getPerformanceTimeStamp() << std::endl;
}
