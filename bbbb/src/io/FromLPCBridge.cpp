#include <io/FromLPCBridge.h>
#include "files/LPCReceiver.h"
#include "network/WebSocketSender.h"

FromLPCBridge::FromLPCBridge() :
    Bridge(new WebSocketSender(Domain::Lpc), new LPCReceiver())
{
}

FromLPCBridge::~FromLPCBridge()
{
}

void FromLPCBridge::sendRibbonPosition(bool m_upperRibon, double value) {
  gint16 messageType = 1024;
  gint16 ribbonId = m_upperRibon ? (gint16)284 : (gint16)289;
  gint16 val = (gint16)value;

  gint16 data[4];
  data[0] = messageType;
  data[1] = 2;
  data[2] = ribbonId;
  data[3] = val;
  auto msg = Glib::Bytes::create(data,8);
  m_sender->send({ msg });
}


