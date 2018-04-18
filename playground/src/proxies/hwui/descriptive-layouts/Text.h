#pragma once

#include "LayoutFactory.h"
#include "Styleable.h"
#include "PropertyOwner.h"
#include "Primitive.h"
#include <proxies/hwui/controls/Label.h>

namespace DescriptiveLayouts
{
  struct Primitive;

  class Text : public Label, public Styleable, public PropertyOwner
  {
      using super = Label;

    public:
      Text(const Primitive &e);
      virtual ~Text();

      void setProperty(PrimitiveProperty key, std::any value);
      void setDirty() override;
      PrimitiveClasses getPrimitiveClass() const override;
      PrimitiveInstances getPrimitiveInstance() const override;

    protected:
      void setFontColor (FrameBuffer &fb) const override;
      Font::Justification getJustification () const override;
      int getFontHeight () const override;
      void drawBackground (FrameBuffer &fb) override;

    private:
      Primitive m_primitive;
  };

}
