#include "Bar.h"

namespace DescriptiveLayouts
{

  Bar::Bar(const TemplateElement &e) :
      Control(e.pos)
  {
    m_length = e.pos.getWidth();
    applyStyles(e.style);
  }

  Bar::~Bar()
  {
  }

  bool Bar::redraw(FrameBuffer &fb)
  {
    auto color = (FrameBuffer::Colors) getStyle(Style::Color);

    Rect r = getPosition();
    r.setLeft(r.getLeft() + m_left);
    r.setWidth(m_length);
    fb.setColor(color);
    fb.fillRect(r);

    return true;
  }

  void Bar::drawBackground (FrameBuffer &fb)
  {
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
    switch(key)
    {
      case ComponentValues::left:
        if(std::exchange(m_left, std::any_cast<int>(value)) != m_left)
          setDirty();
        break;

      case ComponentValues::length:
        if(std::exchange(m_length, std::any_cast<int>(value)) != m_length)
          setDirty();
        break;
    }
  }

}
