#include <proxies/hwui/descriptive-layouts/TemplateEnums.h>
#include "Circle.h"

namespace DescriptiveLayouts
{

    Circle::Circle(const PrimitiveInstance &e) :
            Control(e.relativePosition),
            m_range(0.0, 1.0),
            m_primitive(e),
            m_steps(5),
            m_drawPosition(getPosition().getLeft(), getPosition().getCenter().getY())
    {
    }

    Circle::~Circle()
    {
    }

    bool Circle::redraw(FrameBuffer &fb)
    {
      auto color = (FrameBuffer::Colors) getStyleValue(StyleKey::Color);
      fb.setColor(color);
      fb.fillCircle(m_drawPosition, m_primitive.relativePosition.getHeight() / 2, m_steps);
      return true;
    }

    void Circle::drawBackground(FrameBuffer &fb)
    {
    }

    void Circle::setDirty()
    {
      Control::setDirty();
    }

    void Circle::setProperty(PrimitiveProperty key, std::any value)
    {
      switch(key)
      {
        case PrimitiveProperty::Range:
          if(std::exchange(m_range, std::any_cast < Range > (value)) != m_range)
            m_drawPosition = rangeToPosition(m_range);
            setDirty();
          break;

        case PrimitiveProperty::Visibility:
          setVisible(std::any_cast<bool>(value));
          break;
      }
    }

    const PrimitiveInstance &Circle::getPrimitive() const
    {
      return m_primitive;
    }

    const Point Circle::rangeToPosition(Circle::Range range) const {
      Point p(getPosition().getLeft(), getPosition().getCenter().getY());
      const auto controlPos = range.second;
      const auto totalWidth = getPosition().getWidth();
      const auto stepWidth = totalWidth / 100.0;
      p.moveBy(stepWidth*controlPos*100,0);
      return p;
    }
}
