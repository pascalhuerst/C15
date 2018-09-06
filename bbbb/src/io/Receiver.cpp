#include <io/Receiver.h>

Receiver::Receiver()
{
}

Receiver::~Receiver()
{
}

void Receiver::setCallback(Callback cb)
{
  m_callback =  cb;
}

void Receiver::onDataReceived(const tMessages &msgs)
{
  m_callback(msgs);
}
