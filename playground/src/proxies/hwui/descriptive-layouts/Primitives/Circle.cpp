#include <proxies/hwui/descriptive-layouts/TemplateEnums.h>
#include "Circle.h"
#include "proxies/hwui/descriptive-layouts/PrimitiveInstance.h"

namespace DescriptiveLayouts
{

    Circle::Circle(const PrimitiveInstance &e) :
            Control(e.relativePosition),
            m_range(0.0, 1.0),
            m_primitive(e)

    {
    }

    Circle::~Circle()
    {
    }

    bool Circle::redraw(FrameBuffer &fb)
    {
      auto color = (FrameBuffer::Colors) getStyleValue(StyleKey::Color);

      color = FrameBuffer::C204;

      Rect r = getPosition();

      double controlWidth = r.getWidth();
      double controlLeft = r.getLeft();

      r.setLeft(controlLeft + m_range.first * controlWidth);
      r.normalize();

      auto radius = m_primitive.relativePosition.getHeight();

      fb.setColor(color);
      fb.fillCircle(r, radius);
      fb.fillRect(r);

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
}
