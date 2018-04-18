#include <proxies/hwui/FrameBuffer.h>
#include "Text.h"
#include "Primitive.h"

namespace DescriptiveLayouts
{

  Text::Text(const Primitive &e) :
      super(e.relativePosition),
      m_primitive(e)
  {
  }

  Text::~Text()
  {
  }

  void Text::drawBackground(FrameBuffer &fb)
  {
    auto bg = (FrameBuffer::Colors) getStyleValue(StyleKey::BackgroundColor);

    if(bg != FrameBuffer::Colors::Transparent)
    {
      fb.setColor(bg);
      fb.fillRect(getPosition());
    }
  }

  void Text::setFontColor(FrameBuffer &fb) const
  {
    fb.setColor((FrameBuffer::Colors) getStyleValue(StyleKey::Color));
  }

  Font::Justification Text::getJustification() const
  {
    switch((StyleValues::Alignment) getStyleValue(StyleKey::TextAlign))
    {
      case StyleValues::Alignment::Left:
        return Font::Justification::Left;

      case StyleValues::Alignment::Center:
        return Font::Justification::Center;

      case StyleValues::Alignment::Right:
        return Font::Justification::Right;
    }
    throw std::runtime_error("unkown text align style");
  }

  int Text::getFontHeight() const
  {
    return getStyleValue(StyleKey::FontSize);
  }

  void Text::setProperty(PrimitiveProperty key, std::any value)
  {
    switch(key)
    {
      case PrimitiveProperty::Text:
      {
        DisplayString a = std::any_cast < DisplayString > (value);
        setText(a.first, a.second);
        break;
      }
    }
  }

  void Text::setDirty()
  {
    Control::setDirty();
  }

  const Primitive &Text::getPrimitive() const
  {
    return m_primitive;
  }
}
