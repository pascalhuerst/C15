#include <proxies/hwui/FrameBuffer.h>
#include "Text.h"
#include "Primitive.h"

namespace DescriptiveLayouts
{

  Text::Text(const Primitive &e) :
      super(e.getPosition())
  {
  }

  Text::~Text()
  {
  }

  void Text::drawBackground (FrameBuffer &fb)
  {
  }

  Text::StyleMap Text::getDefaultStyle() const
  {
    static StyleMap defaults =
    {
      { StyleKey::Color, (int) FrameBuffer::C255 },
      { StyleKey::TextAlign, (int) StyleValues::Alignment::Center },
      { StyleKey::FontSize, 9 }
    };

    return defaults;
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
      case PrimitiveProperty::text:
        setText(std::any_cast < Glib::ustring > (value));
        break;
    }
  }

  void Text::setDirty()
  {
    Control::setDirty();
  }

}
