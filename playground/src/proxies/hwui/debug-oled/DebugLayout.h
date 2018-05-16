#pragma once

#include <proxies/hwui/DFBLayout.h>

class DebugLayout : public DFBLayout {
protected:
    std::exception m_exception;
    using super = DFBLayout;
public:
    DebugLayout(std::exception e);
};
