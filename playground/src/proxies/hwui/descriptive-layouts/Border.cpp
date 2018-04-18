#include "Border.h"
#include "Primitive.h"

namespace DescriptiveLayouts
{

  Border::Border(const Primitive &e) :
      Control(e.relativePosition),
      m_primitive(e)
  {
  }

  Border::~Border()
  {
  }

  void Border::drawBackground(FrameBuffer &fb)
  {
  }

  bool Border::redraw(FrameBuffer &fb)
  {
    auto style = (StyleValues::BorderStyle) getStyleValue(StyleKey::BorderStyle);
    auto color = (FrameBuffer::Colors) getStyleValue(StyleKey::Color);

    switch(style)
    {
      case StyleValues::BorderStyle::None:
        return false;

      case StyleValues::BorderStyle::Rounded:
        fb.setColor(color);
        getPosition().drawRounded(fb);
        return true;

      case StyleValues::BorderStyle::Solid:
        fb.setColor(color);
        fb.drawRect(getPosition());
        return true;
    }
    throw std::runtime_error("unkown border style");
    return true;
  }

  void Border::setDirty()
  {
    Control::setDirty();
  }

  const Primitive &Border::getPrimitive() const
  {
    return m_primitive;
  }
}
