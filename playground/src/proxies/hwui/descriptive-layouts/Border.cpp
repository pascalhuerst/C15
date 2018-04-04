#include "Border.h"

namespace DescriptiveLayouts
{

  Border::Border(const TemplateElement &e) :
      Control(e.pos)
  {
    applyStyles(e.style);
  }

  Border::~Border()
  {
  }

  void Border::drawBackground (FrameBuffer &fb)
  {
  }

  bool Border::redraw(FrameBuffer &fb)
  {
    auto style = (StyleValues::BorderStyle) getStyle(Style::BorderStyle);
    auto color = (FrameBuffer::Colors) getStyle(Style::Color);

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

  Border::StyleMap Border::getDefaultStyle() const
  {
    static StyleMap defaults =
    {
     { Style::Color, (int) FrameBuffer::C255 },
     { Style::BorderStyle, (int) StyleValues::BorderStyle::Solid }
    };

    return defaults;
  }
}
