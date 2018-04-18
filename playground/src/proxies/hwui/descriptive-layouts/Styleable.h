#pragma once

#include "Styles.h"
#include "TemplateEnums.h"

namespace DescriptiveLayouts
{
  class Styleable
  {
    public:
      Styleable();
      virtual ~Styleable();

      using StyleMap = std::map<StyleKey, int>;

      void style(LayoutInstances li, ControlClasses cc, ControlInstances ci);
      void applyStyle(const StyleMap &style);
      int getStyleValue(StyleKey s) const;

    protected:
      virtual void setDirty() = 0;

      virtual PrimitiveClasses getPrimitiveClass() const = 0;
      virtual PrimitiveInstances getPrimitiveInstance() const = 0;

    private:
      StyleMap m_currentStyle;
  };

}
