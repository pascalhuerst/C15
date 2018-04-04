#pragma once

#include <proxies/hwui/controls/Control.h>
#include "Stylable.h"
#include "PropertyOwner.h"

namespace DescriptiveLayouts
{

  class Border : public Control, public Stylable, public PropertyOwner
  {
    public:
      Border(const Rect &r);
      virtual ~Border();

    protected:
      bool redraw(FrameBuffer &fb) override;
      void setDirty() override;

    private:
      StyleMap getDefaultStyle() const override;
  };

}
