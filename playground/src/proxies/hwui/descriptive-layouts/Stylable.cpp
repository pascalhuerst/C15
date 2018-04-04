#include "Stylable.h"

namespace DescriptiveLayouts
{

  Stylable::Stylable()
  {
  }

  Stylable::~Stylable()
  {
  }

  void Stylable::applyStyles(Styles styles)
  {
    StyleMap newStyle = getDefaultStyle();

    for(auto &style : styles)
    {
      Style key = style.first;
      int value = style.second;

      newStyle[key] = value;
    }

    if(m_currentStyle != newStyle)
    {
      m_currentStyle = newStyle;
      setDirty();
    }
  }

  int Stylable::getStyle(Style s) const
  {
    return m_currentStyle.at(s);
  }

}
