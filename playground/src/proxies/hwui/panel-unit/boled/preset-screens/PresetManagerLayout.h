#pragma once

#include <proxies/hwui/DFBLayout.h>
#include <proxies/hwui/HWUIEnums.h>
#include <proxies/hwui/panel-unit/boled/preset-screens/controls/InvertedLabel.h>
#include <functional>

class BankAndPresetNumberLabel;

class NumPresetsInBankLabel;

class PresetListBase;
class Setting;
class AutoLoadSelectedPreset;
class ButtonMenu;
class Button;

class PresetManagerLayout : public DFBLayout
{
  private:
    typedef DFBLayout super;

  public:
    PresetManagerLayout(FocusAndMode focusAndMode);
    virtual ~PresetManagerLayout();

    void setFocusAndMode(FocusAndMode focusAndMode);

    virtual bool onButton(int i, bool down, ButtonModifiers modifiers) override;
    virtual bool onRotary(int inc, ButtonModifiers modifiers) override;

    bool animateSelectedPreset(function<void()> cb);

    std::pair<int, int> getSelectedPosition() const;

  private:
    void updateAutoLoadButton(const Setting *setting);
    void setup();
    auto getPresetManager();

    void setupBankFocus();
    void setupPresetFocus();
    void setupPresetEdit();
    void setupPresetSelect();
    void setupPresetStore();
    void setupBankEdit();
    void setupBankSelect();
    void setupBankStore();

    ButtonMenu *m_menu = nullptr;
    Button *m_autoLoad = nullptr;
    NumPresetsInBankLabel *m_numPresetsInBank = nullptr;
    BankAndPresetNumberLabel *m_bankAndPresetNumberLabel = nullptr;
    PresetListBase *m_presets = nullptr;
    FocusAndMode m_focusAndMode;
};

