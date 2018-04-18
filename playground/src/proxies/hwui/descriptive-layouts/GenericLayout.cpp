#include "GenericLayout.h"
#include "GenericControl.h"
#include "Bar.h"
#include "Border.h"
#include "Text.h"

namespace DescriptiveLayouts
{
  GenericLayout::GenericLayout(const LayoutPrototype &prototype) :
      m_prototype(prototype)
  {
  }

  void GenericLayout::init()
  {
    super::init();
    createControls();
  }

  void GenericLayout::createControls()
  {
    for(auto &c : m_prototype.controls)
    {
      auto control = c.instantiate();
      control->style(m_prototype.id);
      control->connect();
      addControl(control);
    }
  }

  bool GenericLayout::onButton(int i, bool down, ::ButtonModifiers modifiers)
  {
    if(down)
    {
      for(const EventSinkMapping &m : m_prototype.sinkMappings)
      {
        if(m.button == i)
        {
          EventSinkBroker::get().fire(m.sink);
          return true;
        }
      }
    }
    return false;
  }

  bool GenericLayout::onRotary(int inc, ::ButtonModifiers modifiers)
  {
    while(inc > 0)
    {
      onButton(ROTARY_PLUS, true, modifiers);
      inc--;
    }

    while(inc < 0)
    {
      onButton(ROTARY_MINUS, true, modifiers);
      inc++;
    }
    return true;
  }
}
