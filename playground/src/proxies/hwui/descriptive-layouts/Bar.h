#pragma once

#include <proxies/hwui/controls/Control.h>
#include "Stylable.h"
#include "PropertyOwner.h"

namespace DescriptiveLayouts
{
  class Primitive;

  class Bar : public Control, public Stylable, public PropertyOwner
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

    private:
      StyleMap getDefaultStyle() const override;

      Range m_range;
  };

}
