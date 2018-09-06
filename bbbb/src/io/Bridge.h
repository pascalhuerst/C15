#pragma once

#include "Receiver.h"
#include <memory>

class Sender;
class Receiver;

class Bridge
{
  public:
    Bridge(Sender *sender, Receiver *receiver);
    virtual ~Bridge();

    virtual void transmit(const Receiver::tMessages &msgs);

   protected:
    std::unique_ptr<Sender> m_sender;
    std::unique_ptr<Receiver> m_receiver;
};

