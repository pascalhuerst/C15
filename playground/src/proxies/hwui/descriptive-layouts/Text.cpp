#include <proxies/hwui/FrameBuffer.h>
#include "Text.h"

namespace DescriptiveLayouts
{

  Text::Text(const TemplateElement &e) :
      super(e.pos)
  {
    applyStyles(e.style);
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
      { Style::Color, (int) FrameBuffer::C255 },
      { Style::TextAlign, (int) StyleValues::Alignment::Center },
      { Style::FontSize, 9 }
    };

    return defaults;
  }

  void Text::setFontColor(FrameBuffer &fb) const
  {
    fb.setColor((FrameBuffer::Colors) getStyle(Style::Color));
  }

  Font::Justification Text::getJustification() const
  {
    switch((StyleValues::Alignment) getStyle(Style::TextAlign))
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
    return getStyle(Style::FontSize);
  }

  void Text::setProperty(ComponentValues key, std::any value)
  {
    switch(key)
    {
      case ComponentValues::text:
        setText(std::any_cast < Glib::ustring > (value));
        break;
    }
  }

  void Text::setDirty()
  {
    Control::setDirty();
  }

}
