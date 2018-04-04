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
    static StyleMap defaults =
    {
      { Style::Color, (int) FrameBuffer::C255 },
      { Style::TextAlign, (int) StyleValues::Alignment::Center },
      { Style::FontSize, 9 }
    };

    return defaults;
  }

  void BasicBuildingBlock::setFontColor(FrameBuffer &fb) const
  {
    fb.setColor((FrameBuffer::Colors) m_currentStyle.at(Style::Color));
  }

  void BasicBuildingBlock::setBackgroundColor(FrameBuffer &fb) const
  {
    //fb.setColor((FrameBuffer::Colors) m_currentStyle.at(Style::BackgroundColor));
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
        pos.setWidth(std::any_cast<tControlPositionValue>(value));
        setPosition(pos);
        break;
      }

      case ComponentValues::left:
      {
        auto pos = getPosition();
        pos.setLeft(std::any_cast<tControlPositionValue>(value));
        setPosition(pos);
        break;
      }
    }
  }

}
