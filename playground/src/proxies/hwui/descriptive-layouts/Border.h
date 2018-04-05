#pragma once

#include <proxies/hwui/controls/Control.h>
#include "Stylable.h"

namespace DescriptiveLayouts
{
  class Primitive;

  class Border : public Control, public Stylable
  {
    public:
      Border(const Primitive &e);
      virtual ~Border();

    protected:
      bool redraw(FrameBuffer &fb) override;
      void setDirty() override;
      void drawBackground (FrameBuffer &fb) override;

    private:
      StyleMap getDefaultStyle() const override;
  };

}
