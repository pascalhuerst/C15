#pragma once

#include <proxies/hwui/descriptive-layouts/LayoutFactory.h>
#include "Styles.h"
#include "TemplateEnums.h"

namespace DescriptiveLayouts
{
  class Stylable
  {
    public:
      Stylable();
      virtual ~Stylable();

      void applyStyle(Style style);
      int getStyleValue(StyleKey s) const;

    protected:
      using StyleMap = std::map<StyleKey, int>;
      virtual StyleMap getDefaultStyle() const = 0;
      virtual void setDirty() = 0;

    private:
      StyleMap m_currentStyle;
  };

}
