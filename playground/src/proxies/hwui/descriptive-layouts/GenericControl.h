#pragma once

#include "ControlInstance.h"
#include "proxies/hwui/controls/ControlWithChildren.h"

namespace DescriptiveLayouts
{
  class GenericControl : public ControlWithChildren
  {
    public:
      GenericControl(const ControlInstance &prototype);
      virtual ~GenericControl();
      void style(LayoutInstances layout);

    private:
      void addPrimitives();
      ControlInstance m_prototype;
  };

}
