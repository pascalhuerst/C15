#pragma once

#include <proxies/hwui/controls/Control.h>
#include "Styleable.h"
#include "Primitive.h"

namespace DescriptiveLayouts
{
  class Primitive;

  class Border : public Control, public Styleable
  {
    public:
      Border(const Primitive &e);
      virtual ~Border();

    protected:
      bool redraw(FrameBuffer &fb) override;
      void setDirty() override;
      void drawBackground(FrameBuffer &fb) override;
      PrimitiveClasses getPrimitiveClass() const override;
      PrimitiveInstances getPrimitiveInstance() const override;

    private:
      Primitive m_primitive;
  };

}
