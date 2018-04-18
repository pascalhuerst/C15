#pragma once

#include <bitset>
#include <variant>

enum class UIFocus
{
  Any, Sound, Parameters, Presets, Banks, Setup, Unchanged
};

enum class UIMode
{
  Any, Select, Store, Edit, Info, Unchanged
};

enum class UIFocusAndModeDetail
{
  Any,
  Init,
  MCSelect,
  MCAmount,
  MCPosition,
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

struct FocusAndMode
{
    FocusAndMode (UIFocus f, UIMode m) :
        focus (f),
        mode (m),
        detail (UIFocusAndModeDetail::Init)
    {
    }

    FocusAndMode(UIFocus f, UIMode m, UIFocusAndModeDetail d) : focus{f}, mode{m}, detail{d}
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
    UIFocusAndModeDetail detail;
};


