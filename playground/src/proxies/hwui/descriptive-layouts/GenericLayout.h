#pragma once

#include <proxies/hwui/DFBLayout.h>
#include "LayoutPrototype.h"


namespace DescriptiveLayouts
{
  class PropertyOwner;

  class GenericLayout : public DFBLayout
  {
      using super = DFBLayout;

    public:
      GenericLayout(const LayoutPrototype &prototype);

      void init () override;
      bool onButton (int i, bool down, ::ButtonModifiers modifiers) override;
      bool onRotary (int inc, ::ButtonModifiers modifiers) override;

    private:
      void createControls();

      LayoutPrototype m_prototype;
  };
}
