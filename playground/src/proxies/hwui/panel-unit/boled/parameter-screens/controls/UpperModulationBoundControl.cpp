#include <Application.h>
#include <groups/MacroControlsGroup.h>
#include <libundo/undo/Scope.h>
#include <parameters/MacroControlParameter.h>
#include <parameters/ModulateableParameter.h>
#include <presets/EditBuffer.h>
#include <presets/PresetManager.h>
#include <proxies/hwui/HWUI.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/UpperModulationBoundControl.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/UpperModulationBoundLabel.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/UpperModulationBoundSlider.h>

UpperModulationBoundControl::UpperModulationBoundControl(const Rect &r) :
    super(r)
{
  auto height = r.getHeight() / 2;
  addControl(new UpperModulationBoundSlider(Rect(0, 0, r.getWidth(), height)));
  addControl(new UpperModulationBoundLabel(Rect(0, height, r.getWidth(), height)));
}

UpperModulationBoundControl::~UpperModulationBoundControl()
{
}

bool UpperModulationBoundControl::onRotary(int inc, ButtonModifiers modifiers)
{
  auto editBuffer = Application::get().getPresetManager()->getEditBuffer();

  if(auto modulatedParam = dynamic_cast<ModulateableParameter*>(editBuffer->getSelected()))
  {
    auto mc = modulatedParam->getModulationSource();
    auto mcID = MacroControlsGroup::modSrcToParamID(mc);

    if(auto mcParam = dynamic_cast<MacroControlParameter*>(editBuffer->findParameterByID(mcID)))
    {
      auto range = modulatedParam->getModulationRange(true);

      if(modulatedParam->isBiPolar())
      {
        range.first = range.first * 2 - 1;
        range.second = range.second * 2 - 1;
      }

      auto maxCP = modulatedParam->getValue().getScaleConverter()->getControlPositionRange().getMax();
      auto wasClipped = range.second >= maxCP;

      auto srcValue = mcParam->getControlPositionValue();
      auto fine = modifiers[ButtonModifier::FINE];
      auto fineDenominator = modulatedParam->getModulationAmountFineDenominator();
      auto coarseDenominator = modulatedParam->getModulationAmountCoarseDenominator();
      auto denominator = fine ? fineDenominator : coarseDenominator;
      auto newRight = (round(range.second * denominator) + inc) / denominator;
      auto isClipped = newRight >= maxCP;

      if(wasClipped && isClipped)
        return true;

      auto newModAmount = newRight - range.first;
      auto newValue = range.first + newModAmount * srcValue;

      if(modulatedParam->isBiPolar())
        newModAmount /= 2;

      auto &undoScope = modulatedParam->getUndoScope();
      auto scope = undoScope.startContinuousTransaction(modulatedParam, "Set '%0'", modulatedParam->getGroupAndParameterName());
      auto transaction = scope->getTransaction();

      modulatedParam->undoableSetModAmount(transaction, newModAmount);
      modulatedParam->setCPFromHwui(transaction, newValue);
      return true;
    }
  }
  return false;
}
