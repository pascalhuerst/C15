#pragma once

#include <proxies/hwui/DFBLayout.h>
#include <proxies/hwui/HWUIEnums.h>

class Rect;

class ScrollArea;
class Scrollable;

class InfoLayout : public DFBLayout
{
  private:
    typedef DFBLayout super;

  public:
    InfoLayout ();

    virtual void init() override;

  protected:
    virtual bool onRotary (int inc, ButtonModifiers modifiers) override;
    virtual bool onButton (int i, bool down, ButtonModifiers modifiers) override;

    virtual void addModuleCaption() = 0;
    virtual void addHeadline() = 0;
    virtual void addInfoLabel() = 0;
    virtual Scrollable* createScrollableContent() = 0;
    virtual Rect getScrollableAreaRect () const;

    void scrollToMax();

  private:
    ScrollArea *m_scrollArea = nullptr;
};
