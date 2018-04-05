#pragma once

#include "TemplateEnums.h"
#include <proxies/hwui/HWUI.h>

namespace DescriptiveLayouts
{

  struct StyleAttribute
  {
      StyleKey key;
      int value;
  };

  using Style = std::list<StyleAttribute>;

  struct StyleMapping
  {
      const char *selector;
      Style style;
  };

  class StyleSheet
  {
    public:
      static StyleSheet &get();

    private:
      StyleSheet();

      void add(UIFocus f, UIMode m, LayoutInstances lInst, ControlClasses cClass, ControlInstances cInst, PrimitiveClasses pClass,
               PrimitiveInstances pInst, Style s);
  };

}
