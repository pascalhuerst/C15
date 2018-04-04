#pragma once

#include <proxies/hwui/controls/Control.h>
#include "Stylable.h"

namespace DescriptiveLayouts
{

  class Border : public Control, public Stylable
  {
    public:
      Border(const TemplateElement &e);
      virtual ~Border();

    protected:
      bool redraw(FrameBuffer &fb) override;
      void setDirty() override;
      void drawBackground (FrameBuffer &fb) override;

    private:
      StyleMap getDefaultStyle() const override;
  };

}
