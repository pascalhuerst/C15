#include "ControlInstance.h"
#include "GenericControl.h"
#include "proxies/hwui/panel-unit/boled/parameter-screens/controls/ParameterEditButtonMenu.h"

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

  Control* ControlInstance::instantiate() const
  {
    if(controlClass == "ParameterEditButtonMenu")
    {
      return new ParameterEditButtonMenu(Rect(position.getX(), position.getY(), 64, 128));
    }
    return new GenericControl(*this);
  }
}
