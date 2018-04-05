#include "Stylable.h"

namespace DescriptiveLayouts
{

  Stylable::Stylable()
  {
  }

  Stylable::~Stylable()
  {
  }

  void Stylable::applyStyle(Style style)
  {
    StyleMap newStyle = getDefaultStyle();

    for(auto &style : style)
    {
      auto key = style.key;
      auto value = style.value;
      newStyle[key] = value;
    }

    if(m_currentStyle != newStyle)
    {
      m_currentStyle = newStyle;
      setDirty();
    }
  }

  int Stylable::getStyleValue(StyleKey s) const
  {
    return m_currentStyle.at(s);
  }

}
