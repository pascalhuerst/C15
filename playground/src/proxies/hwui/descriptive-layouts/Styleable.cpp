#include "Styleable.h"
#include "Application.h"
#include <proxies/hwui/HWUI.h>


namespace DescriptiveLayouts
{

  Styleable::Styleable()
  {
  }

  Styleable::~Styleable()
  {
  }

  void Styleable::style(LayoutInstances li, ControlClasses cc, ControlInstances ci)
  {
    const StyleSheet s = StyleSheet::get();
    auto fam = Application::get().getHWUI()->getFocusAndMode();
    s.applyStyle(fam.focus, fam.mode, fam.detail, li, cc, ci, getPrimitiveClass(), getPrimitiveInstance(), this);
  }

  void Styleable::applyStyle(const StyleMap &style)
  {
    if(m_currentStyle != style)
    {
      m_currentStyle = style;
      setDirty();
    }
  }

  int Styleable::getStyleValue(StyleKey s) const
  {
    return m_currentStyle.at(s);
  }

}
