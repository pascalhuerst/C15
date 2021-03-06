#pragma once

#include <proxies/hwui/HWUIEnums.h>

class SetupLayout;

class SetupEditor
{
  public:
    SetupEditor ();
    virtual ~SetupEditor ();

    virtual bool onButton (int i, bool down, ButtonModifiers modifiers);
    virtual bool onRotary (int inc, ButtonModifiers modifiers);

    void setSetupLayout (SetupLayout* layout);
    void diveUp ();

  private:
    SetupLayout *m_setupLayout = nullptr;
};
