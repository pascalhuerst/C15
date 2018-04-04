#include "Bar.h"

namespace DescriptiveLayouts
{

  Bar::Bar(const Rect &rect) :
      Control(rect)
  {
  }

  Bar::~Bar()
  {
  }

  bool Bar::redraw(FrameBuffer &fb)
  {
    auto color = (FrameBuffer::Colors) getStyle(Style::Color);
/*
    switch(style)
    {
      case StyleValues::BorderStyle::None:
        return false;

      case StyleValues::BorderStyle::Dotted:
        throw std::runtime_error("not yet supported: dotted border");

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
    */
    return true;
  }

  void Bar::setDirty()
  {
    Control::setDirty();
  }

  Bar::StyleMap Bar::getDefaultStyle() const
  {
    static StyleMap defaults =
    {
     { Style::Color, (int) FrameBuffer::C255 }
    };

    return defaults;
  }

  void Bar::setProperty(ComponentValues key, std::any value)
  {

  }
}
