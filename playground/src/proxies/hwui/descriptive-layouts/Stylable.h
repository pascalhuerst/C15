#pragma once

#include <proxies/hwui/descriptive-layouts/LayoutFactory.h>

namespace DescriptiveLayouts
{

  class Stylable
  {
    public:
      Stylable();
      virtual ~Stylable();

      using Styles = std::list<std::pair<Style, int>>;
      void applyStyles(Styles styles);
      int getStyle(Style s) const;

    protected:
      using StyleMap = std::map<Style, int>;
      virtual StyleMap getDefaultStyle() const = 0;
      virtual void setDirty() = 0;

    private:
      StyleMap m_currentStyle;
  };

}
