#include "BasicBuildingBlock.h"
#include <proxies/hwui/FrameBuffer.h>

namespace DescriptiveLayouts
{

  BasicBuildingBlock::BasicBuildingBlock(const TemplateElement &e) :
      super(e.pos)
  {
    applyStyles(e.style);
  }

  BasicBuildingBlock::~BasicBuildingBlock()
  {
  }

  bool BasicBuildingBlock::redraw(FrameBuffer &fb)
  {
    bool redrew = super::redraw(fb);
    redrew |= drawBorder(fb);
    return redrew;
  }

  bool BasicBuildingBlock::drawBorder(FrameBuffer &fb)
  {
    auto style = (StyleValues::BorderStyle) m_currentStyle.at(Style::BorderStyle);
    auto color = (FrameBuffer::Colors) m_currentStyle.at(Style::BorderColor);

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
  }

  void BasicBuildingBlock::applyStyles(Styles styles)
  {
    StyleMap newStyle = getDefaultStyle();

    for(auto &style : styles)
    {
      Style key = style.first;
      int value = style.second;

      newStyle[key] = value;
    }

    if(m_currentStyle != newStyle)
    {
      m_currentStyle = newStyle;
      setDirty();
    }
  }

  BasicBuildingBlock::StyleMap BasicBuildingBlock::getDefaultStyle() const
  {
    static StyleMap defaults = { { Style::ForegroundColor, (int) FrameBuffer::C255 }, { Style::BackgroundColor, (int) FrameBuffer::C43 }, {
        Style::BorderColor, (int) FrameBuffer::C43 },
                                 { Style::TextAlign, (int) StyleValues::Alignment::Center }, { Style::FontSize, 9 }, {
                                     Style::BorderStyle, (int) StyleValues::BorderStyle::None },
                                 { Style::BorderWidth, 1 } };

    return defaults;
  }

  void BasicBuildingBlock::setFontColor(FrameBuffer &fb) const
  {
    fb.setColor((FrameBuffer::Colors) m_currentStyle.at(Style::ForegroundColor));
  }

  void BasicBuildingBlock::setBackgroundColor(FrameBuffer &fb) const
  {
    fb.setColor((FrameBuffer::Colors) m_currentStyle.at(Style::BackgroundColor));
  }

  Font::Justification BasicBuildingBlock::getJustification() const
  {
    switch((StyleValues::Alignment) m_currentStyle.at(Style::TextAlign))
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

  int BasicBuildingBlock::getFontHeight() const
  {
    return m_currentStyle.at(Style::FontSize);
  }

  void BasicBuildingBlock::setProperty(ComponentValues key, std::any value)
  {
    switch(key)
    {
      case ComponentValues::text:
        setText(std::any_cast < Glib::ustring > (value));
        break;

      case ComponentValues::length:
      {
        auto pos = getPosition();
        pos.setWidth(std::any_cast<int>(value));
        setPosition(pos);
        break;
      }

      case ComponentValues::left:
      {
        auto pos = getPosition();
        pos.setLeft(std::any_cast<int>(value));
        setPosition(pos);
        break;
      }
    }
  }

}
