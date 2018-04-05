#include "Bar.h"
#include "Primitive.h"

namespace DescriptiveLayouts
{

  Bar::Bar(const Primitive &e) :
      Control(e.getPosition()),
      m_range(0.0, 1.0)
  {
  }

  Bar::~Bar()
  {
  }

  bool Bar::redraw(FrameBuffer &fb)
  {
    auto color = (FrameBuffer::Colors) getStyleValue(StyleKey::Color);

    Rect r = getPosition();

    double controlWidth = r.getWidth();
    double controlLeft = r.getLeft();

    r.setLeft(controlLeft + m_range.first * controlWidth);
    r.setWidth(m_range.second * controlWidth);
    r.normalize();

    fb.setColor(color);
    fb.fillRect(r);

    return true;
  }

  void Bar::drawBackground(FrameBuffer &fb)
  {
  }

  void Bar::setDirty()
  {
    Control::setDirty();
  }

  Bar::StyleMap Bar::getDefaultStyle() const
  {
    static StyleMap defaults = { { StyleKey::Color, (int) FrameBuffer::C255 } };

    return defaults;
  }

  void Bar::setProperty(PrimitiveProperty key, std::any value)
  {
    switch(key)
    {
      case PrimitiveProperty::range:
        if(std::exchange(m_range, std::any_cast < Range > (value)) != m_range)
          setDirty();
        break;

      case PrimitiveProperty::visibility:
        setVisible(std::any_cast<bool>(value));
        break;
    }
  }

}
