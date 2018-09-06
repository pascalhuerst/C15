#include <io/Bridge.h>
#include "Receiver.h"
#include "Sender.h"
#include <functional>

Bridge::Bridge(Sender *sender, Receiver *receiver) :
    m_sender(sender),
    m_receiver(receiver)
{
  Receiver::Callback cb = std::bind(&Bridge::transmit, this, std::placeholders::_1);
  m_receiver->setCallback(cb);
}

Bridge::~Bridge()
{
}

void Bridge::transmit(const Receiver::tMessages &msgs)
{
  for(auto &msg : msgs)
    m_sender->send(msg);
}

