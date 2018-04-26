#pragma once

#include "TemplateEnums.h"
#include "ControlClass.h"

namespace DescriptiveLayouts
{
  class GenericControl;

  struct ControlInstance
  {

    public:
      ControlInstance(ControlInstances controlInstance, ControlClasses control, Point position, EventSources eventSource = EventSources::None,
                      PrimitiveInstances eventTarget = PrimitiveInstances::None);

      GenericControl* instantiate() const;

    private:
      ControlInstances controlInstance;
      ControlClasses controlClass;
      Point position;
      EventSources eventSource;
      PrimitiveInstances eventTarget;

      friend class GenericControl;
  };
}
