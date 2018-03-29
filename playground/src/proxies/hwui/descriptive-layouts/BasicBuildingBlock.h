#pragma once

#include "LayoutFactory.h"
#include <proxies/hwui/controls/Label.h>

namespace DescriptiveLayouts
{
  struct TemplateElement;

  class BasicBuildingBlock : public Label
  {
      using super = Label;

    public:
      BasicBuildingBlock(const TemplateElement &e);
      virtual ~BasicBuildingBlock();

      void setProperty(ComponentValues key, std::any value);

      using Styles = std::list<std::pair<Style, int>>;
      void applyStyles(Styles styles);

    protected:
      bool redraw (FrameBuffer &fb) override;
      void setFontColor (FrameBuffer &fb) const override;
      void setBackgroundColor (FrameBuffer &fb) const override;
      Font::Justification getJustification () const override;
      int getFontHeight () const override;

    private:
      using StyleMap = std::map<Style, int>;
      StyleMap getDefaultStyle() const;
      void applyStyle(Style key, int value);
      bool drawBorder(FrameBuffer &fb);

      StyleMap m_currentStyle;
  };

}
