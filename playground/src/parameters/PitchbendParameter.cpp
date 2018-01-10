#include "PitchbendParameter.h"
#include <proxies/hwui/panel-unit/boled/parameter-screens/PlayControlParameterLayouts.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/ParameterInfoLayout.h>


PhysicalControlParameter::ReturnMode PitchbendParameter::getReturnMode() const
{
  return ReturnMode::Center;
}

DFBLayout *PitchbendParameter::createLayout (FocusAndMode focusAndMode) const
{
  switch (focusAndMode.mode)
  {
    case UIMode::Info:
      return new ParameterInfoLayout ();

    default:
      return new PitchbendParameterSelectLayout2 ();
  }

  return super::createLayout (focusAndMode);
}
