#include "DFBLayout.h"

#include "Application.h"
#include "proxies/hwui/HWUI.h"
#include "proxies/hwui/controls/Control.h"
#include "controls/Button.h"
#include "proxies/hwui/panel-unit/PanelUnit.h"
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>

#include <proxies/hwui/buttons.h>
#include "ButtonRepeat.h"

DFBLayout::DFBLayout (OLEDProxy &oled) :
    m_oled (oled)
{
}

DFBLayout::~DFBLayout ()
{
}

OLEDProxy &DFBLayout::getOLEDProxy()
{
  return m_oled;
}

bool DFBLayout::redrawLayout ()
{
  bool doRedraw = m_clear || isDirty ();

  if (m_clear)
  {
    m_clear = false;
    m_oled.clear ();
  }

  if (doRedraw)
    return redraw (getFrameBuffer ());

  return doRedraw;
}

FrameBuffer &DFBLayout::getFrameBuffer ()
{
  return FrameBuffer::get ();
}

bool DFBLayout::onButton (int i, bool down, ButtonModifiers modifiers)
{
  if (i == BUTTON_INC || i == BUTTON_DEC)
  {
    if (down)
    {
      installButtonRepeat ([ = ]()
      {
        int direction = (i == BUTTON_INC) ? 1 : - 1;
        onRotary (direction, modifiers);
      });
    }
    else
    {
      removeButtonRepeat ();
    }

    return true;
  }

  return false;
}

bool DFBLayout::onRotary (int inc, ButtonModifiers modifiers)
{
  return false;
}

void DFBLayout::setDirty ()
{
  ControlOwner::setAllDirty ();
  m_clear = true;
}

void DFBLayout::installButtonRepeat (function<void ()> cb)
{
  m_buttonRepeat.reset (new ButtonRepeat (cb));
}

void DFBLayout::removeButtonRepeat ()
{
  m_buttonRepeat.reset ();
}

bool DFBLayout::isResolutionFine () const
{
  return Application::get ().getHWUI ()->isResolutionFine ();
}
