#include "ControlInstance.h"
#include "GenericControl.h"

namespace DescriptiveLayouts
{
  ControlInstance::ControlInstance(ControlInstances controlInstance, ControlClasses control, Point position, EventSources eventSource,
                                   PrimitiveInstances eventTarget) :
      controlInstance(controlInstance),
      controlClass(control),
      position(position),
      eventSource(eventSource),
      eventTarget(eventTarget)
  {
  }

  GenericControl* ControlInstance::instantiate() const
  {
    return new GenericControl(*this);
  }
}
