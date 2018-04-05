#pragma once

#include "TemplateEnums.h"
#include "ControlPrototype.h"

namespace DescriptiveLayouts
{
  class GenericControl;

  struct ControlInstance
  {

    public:
      ControlInstance(ControlInstances controlInstance, ControlPrototype control, Point position, EventSources eventSource = EventSources::none,
                      PrimitiveInstances eventTarget = PrimitiveInstances::none);

      GenericControl* instantiate() const;

    private:
      ControlInstances controlInstance;
      ControlPrototype control;
      Point position;
      EventSources eventSource;
      PrimitiveInstances eventTarget;

      friend class GenericControl;
  };
}
