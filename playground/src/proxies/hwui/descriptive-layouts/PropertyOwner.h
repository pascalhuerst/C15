#pragma once

#include <proxies/hwui/descriptive-layouts/LayoutFactory.h>
#include <any>

namespace DescriptiveLayouts
{

  class PropertyOwner
  {
    public:
      virtual void setProperty(ComponentValues key, std::any value) = 0;
  };

}
