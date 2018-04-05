#pragma once

#include "LayoutFactory.h"
#include "Stylable.h"
#include "PropertyOwner.h"
#include <proxies/hwui/controls/Label.h>

namespace DescriptiveLayouts
{
  struct Primitive;

  class Text : public Label, public Stylable, public PropertyOwner
  {
      using super = Label;

    public:
      Text(const Primitive &e);
      virtual ~Text();

      void setProperty(PrimitiveProperty key, std::any value);
      void setDirty() override;

    protected:
      void setFontColor (FrameBuffer &fb) const override;
      Font::Justification getJustification () const override;
      int getFontHeight () const override;
      StyleMap getDefaultStyle() const override;
      void drawBackground (FrameBuffer &fb) override;
  };

}
