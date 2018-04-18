#pragma once

#include <proxies/hwui/controls/Control.h>
#include "PropertyOwner.h"
#include "Styleable.h"
#include "Primitive.h"

namespace DescriptiveLayouts
{
  class Primitive;

  class Bar : public Control, public Styleable, public PropertyOwner
  {
    public:
      Bar(const Primitive &e);
      virtual ~Bar();

      using Range = std::pair<tControlPositionValue, tControlPositionValue>;

      void setProperty(PrimitiveProperty key, std::any value) override;

    protected:
      bool redraw(FrameBuffer &fb) override;
      void setDirty() override;
      void drawBackground (FrameBuffer &fb) override;

      PrimitiveClasses getPrimitiveClass() const;
      PrimitiveInstances getPrimitiveInstance() const;

    private:
      Primitive m_primitive;
      Range m_range;
  };

}
