#include "ControlInstance.h"
#include "GenericControl.h"

namespace DescriptiveLayouts
{
  ControlInstance::ControlInstance(ControlInstances controlInstance, ControlClass control, Point position, EventSources eventSource,
                                   PrimitiveInstances eventTarget) :
      controlInstance(controlInstance),
      control(control),
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
