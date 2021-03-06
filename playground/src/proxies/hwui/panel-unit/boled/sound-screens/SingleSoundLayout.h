#pragma once

#include <proxies/hwui/DFBLayout.h>
#include <proxies/hwui/HWUIEnums.h>
#include <proxies/hwui/panel-unit/boled/preset-screens/controls/AnyParameterLockedIndicator.h>

class SingleSoundEditMenu;
class SingleSoundMenu;

class SingleSoundLayout : public DFBLayout
{
  private:
    typedef DFBLayout super;

  public:
    SingleSoundLayout (FocusAndMode focusAndMode);
    virtual ~SingleSoundLayout ();

    virtual bool onButton (int i, bool down, ButtonModifiers modifiers) override;
    virtual bool onRotary (int inc, ButtonModifiers modifiers) override;

  private:
    enum class Column
    {
      None, Init, Randomize, TranstionTime, Edit
    };

    void toggleEditMenu(bool inEdit);
    void toggleColumn (Column c);
    void setup ();
    void randomize ();
    void action ();
    void setDefault ();
    void initSound ();

    Button *m_initButton = nullptr;

    Control *m_randomizeLabel = nullptr;
    Control *m_randomizeSlider = nullptr;
    Control *m_randomizeAmount = nullptr;
    Button *m_randomButton = nullptr;
    Button* m_emptyButton = nullptr;

    Control *m_transitionTimeLabel = nullptr;
    Control *m_transitionTimeSlider = nullptr;
    Control *m_transitionTimeAmount = nullptr;
    Button *m_transitionTimeButton = nullptr;
    AnyParameterLockedIndicator* m_paramLocked = nullptr;

    SingleSoundEditMenu *m_initMenu = nullptr;

    Control *m_edit = nullptr;

    Column m_selectedColumn = Column::None;
};

