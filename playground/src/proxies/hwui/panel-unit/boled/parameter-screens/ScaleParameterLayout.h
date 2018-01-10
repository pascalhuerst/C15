#pragma once

#include <proxies/hwui/panel-unit/boled/parameter-screens/UnmodulatebaleParameterLayouts.h>

class ScaleParameterLayout : public UnmodulateableParameterSelectLayout2
{
    typedef UnmodulateableParameterSelectLayout2 super;

  public:
    using super::super;

    virtual void init () override;

  protected:
    void addButtons () override;
    bool onButton (int i, bool down, ButtonModifiers modifiers) override;
    void reset();
    void selectParameter (int inc);
};

