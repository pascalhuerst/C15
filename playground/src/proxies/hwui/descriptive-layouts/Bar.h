#pragma once

#include <proxies/hwui/controls/Control.h>
#include "Stylable.h"
#include "PropertyOwner.h"

namespace DescriptiveLayouts
{

  class Bar : public Control, public Stylable, public PropertyOwner
  {
    public:
      Bar(const Rect &r);
      virtual ~Bar();

      void setProperty(ComponentValues key, std::any value) override;

    protected:
      bool redraw(FrameBuffer &fb) override;
      void setDirty() override;

    private:
      StyleMap getDefaultStyle() const override;
  };

}
