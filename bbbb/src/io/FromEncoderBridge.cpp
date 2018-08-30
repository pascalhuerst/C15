#include <io/files/FileIOReceiver.h>
#include <io/FromEncoderBridge.h>
#include <io/network/WebSocketSender.h>
#include <io/TurnAroundStopWatch.h>

FromEncoderBridge::FromEncoderBridge() :
    Bridge(new WebSocketSender(Domain::Rotary),
           new FileIOReceiver("/dev/espi_encoder", 1))
{
}

FromEncoderBridge::~FromEncoderBridge()
{
}

void FromEncoderBridge::sendRotary(int8_t inc)
{
  static uint32_t packetID = 0;

  int8_t m[5];
  m[0] = inc;
  memcpy(&m[1], &packetID, 4);
  auto msg = Glib::Bytes::create(m, 5);
  m_sender->send(msg);
  TurnAroundStopWatch::get(packetID).start();
  packetID++;
}
