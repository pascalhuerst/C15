#pragma once

#include <io/Bridge.h>

class ToOledsBridge : public Bridge
{
    using super = Bridge;

  public:
    ToOledsBridge();
    virtual ~ToOledsBridge();

    void transmit(Receiver::tMessage msg) override;

};

