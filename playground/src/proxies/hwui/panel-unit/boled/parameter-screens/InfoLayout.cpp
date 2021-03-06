#include "InfoLayout.h"
#include "controls/MultiLineLabel.h"
#include "controls/ScrollArea.h"
#include <proxies/hwui/panel-unit/boled/preset-screens/PresetManagerLayout.h>
#include <proxies/hwui/controls/Rect.h>
#include <proxies/hwui/controls/Label.h>
#include <proxies/hwui/buttons.h>
#include <proxies/hwui/HWUI.h>
#include <Application.h>

InfoLayout::InfoLayout () :
    super (Application::get ().getHWUI ()->getPanelUnit ().getEditPanel ().getBoled ())
{

}

void InfoLayout::init ()
{
  super::init ();

  addModuleCaption ();
  addHeadline ();
  addInfoLabel ();

  m_scrollArea = addControl (new ScrollArea (createScrollableContent (), getScrollableAreaRect ()));
}

Rect InfoLayout::getScrollableAreaRect () const
{
  return Rect (0, 14, 256, 50);
}

bool InfoLayout::onRotary (int inc, ButtonModifiers modifiers)
{
  m_scrollArea->scroll (inc*2);
  return true;
}

void InfoLayout::scrollToMax ()
{
  m_scrollArea->scroll (std::numeric_limits<int>::max () / 2);
}

bool InfoLayout::onButton (int i, bool down, ButtonModifiers modifiers)
{
  if (down)
  {
    switch (i)
    {
    case BUTTON_A:
      if (Application::get().getHWUI()->getFocusAndMode().mode == UIMode::Info)
      {
        if (Application::get().getHWUI()->getFocusAndMode().focus == UIFocus::Presets)
        {
          Application::get().getHWUI()->undoableSetFocusAndMode(FocusAndMode(UIFocus::Banks, UIMode::Info));
        }
        else if (Application::get().getHWUI()->getFocusAndMode().focus == UIFocus::Banks)
        {
          Application::get().getHWUI()->undoableSetFocusAndMode(FocusAndMode(UIFocus::Presets, UIMode::Info));
        }
        return true;
      }
      break;
      case BUTTON_PRESET:
        if (Application::get ().getHWUI ()->getFocusAndMode().focus == UIFocus::Presets)
        {
          Application::get ().getHWUI ()->undoableSetFocusAndMode (UIMode::Select);
        }
        else
        {
          Application::get ().getHWUI ()->undoableSetFocusAndMode (UIFocus::Presets);
        }
        return true;

      case BUTTON_STORE:
        Application::get ().getHWUI ()->undoableSetFocusAndMode (FocusAndMode (UIFocus::Presets, UIMode::Store));
        return true;

      case BUTTON_EDIT:
        Application::get ().getHWUI ()->undoableSetFocusAndMode (UIMode::Edit);
        return true;

      case BUTTON_INFO:
      case BUTTON_ENTER:
        Application::get ().getHWUI ()->undoableSetFocusAndMode (UIMode::Select);
        return true;
    }
  }

  return super::onButton (i, down, modifiers);
}

