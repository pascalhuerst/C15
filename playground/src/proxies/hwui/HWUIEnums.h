#pragma once

#include <bitset>
#include <variant>

enum class UIFocus
{
  Sound, Parameters, Presets, Banks, Setup, Unchanged
};

enum class UIMode
{
  Select, Store, Edit, Info, Unchanged
};

enum ButtonModifier
{
  FINE, SHIFT, NUM_MODIFIERS
};

class ButtonModifiers : public std::bitset<ButtonModifier::NUM_MODIFIERS>
{
    typedef std::bitset<ButtonModifier::NUM_MODIFIERS> super;

  public:
    ButtonModifiers ()
    {
    }

    ButtonModifiers (unsigned long long dontCallme);

    ButtonModifiers (ButtonModifier mod)
    {
      set (mod);
    }
};

enum class EmptyDetail {

};

enum class ParameterDetail {
    select, none, mcassign, mcpos, mcamount
};

enum class PresetDetail {
    bank, preset, store
};

typedef std::variant<ParameterDetail, PresetDetail, EmptyDetail> LayoutDetail;

struct FocusAndMode
{
    FocusAndMode (UIFocus f, UIMode m) :
        focus (f),
        mode (m),
        detail{}
    {
    }

    FocusAndMode(UIFocus f, UIMode m, LayoutDetail d) : focus{f}, mode{m}, detail{d}
    {
    }

    FocusAndMode () :
        FocusAndMode (UIFocus::Parameters)
    {
    }

    FocusAndMode (UIFocus f) :
        FocusAndMode (f, UIMode::Unchanged)
    {
    }

    FocusAndMode (UIMode m) :
        FocusAndMode (UIFocus::Unchanged, m)
    {
    }

    bool operator== (const FocusAndMode &other) const
    {
      return other.focus == focus && other.mode == mode && other.detail == detail;
    }

    bool operator!= (const FocusAndMode &other) const
    {
      return !(*this == other);
    }

    void fixUnchanged (const FocusAndMode &other)
    {
      if (focus == UIFocus::Unchanged)
        focus = other.focus;

      if (mode == UIMode::Unchanged)
        mode = other.mode;
    }

    UIFocus focus;
    UIMode mode;
    LayoutDetail detail;
};


