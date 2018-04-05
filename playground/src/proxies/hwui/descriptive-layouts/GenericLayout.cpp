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

  void GenericLayout::init ()
  {
    super::init();
    createControls();
  }

  void GenericLayout::createControls()
  {
    for(auto &c : m_prototype.controls)
    {
      auto control  = c.instantiate();
      control->style(m_prototype.id);
      addControl(control);
    }
  }
}
