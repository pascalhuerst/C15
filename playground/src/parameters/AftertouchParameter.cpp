#include "AftertouchParameter.h"
#include <proxies/hwui/panel-unit/boled/parameter-screens/PlayControlParameterLayouts.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/ParameterInfoLayout.h>

PhysicalControlParameter::ReturnMode AftertouchParameter::getReturnMode() const
{
  return ReturnMode::Zero;
}

DFBLayout *AftertouchParameter::createLayout (FocusAndMode focusAndMode) const
{
  switch (focusAndMode.mode)
  {
    case UIMode::Info:
      return new ParameterInfoLayout ();

    default:
      return new AftertouchParameterSelectLayout2 ();
  }

  return super::createLayout (focusAndMode);
}
