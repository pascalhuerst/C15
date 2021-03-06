#include <Application.h>
#include <device-settings/EnumSetting.h>
#include <device-settings/Settings.h>
#include <proxies/hwui/buttons.h>
#include <proxies/hwui/HWUI.h>
#include <proxies/hwui/HWUIEnums.h>
#include <proxies/hwui/panel-unit/PanelUnit.h>
#include <proxies/hwui/panel-unit/PanelUnitParameterEditMode.h>
#include <proxies/hwui/panel-unit/PanelUnitPresetMode.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/ModulateableParameterLayouts.h>
#include <proxies/hwui/TwoStateLED.h>
#include <xml/FileOutStream.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <parameters/ModulationRoutingParameter.h>
#include <parameters/PhysicalControlParameter.h>
#include <parameters/MacroControlParameter.h>
#include <groups/MacroControlsGroup.h>
#include <http/UndoScope.h>

PanelUnit::PanelUnit()
{
  const int numLEDs = 96;

  m_leds.reserve(numLEDs);

  for(int i = 0; i < numLEDs; i++)
    m_leds.emplace_back(new TwoStateLED(i));

  m_macroControlAssignmentStateMachine.registerHandler(MacroControlAssignmentStates::Selected, [=]()
  {
    auto editBuffer = Application::get().getPresetManager()->getEditBuffer();
    auto p = editBuffer->getSelected();

    if(auto mrp = dynamic_cast<ModulationRoutingParameter*>(p))
    {
      mrp->getSourceParameter()->setUiSelectedModulationRouter(p->getID());
    }

    auto currentMc = m_macroControlAssignmentStateMachine.getCurrentMCParameter();
    editBuffer->undoableSelectParameter(currentMc);
    return true;
  });

  m_macroControlAssignmentStateMachine.registerHandler(MacroControlAssignmentStates::Assign, [=]()
  {
    auto editBuffer = Application::get().getPresetManager()->getEditBuffer();
    auto selParam = editBuffer->getSelected();
    auto mc = MacroControlsGroup::paramIDToModSrc(selParam->getID());

    auto targetId = m_macroControlAssignmentStateMachine.getCurrentModulateableParameter();
    auto target = editBuffer->findParameterByID(targetId);

    if(auto modParam = dynamic_cast<ModulateableParameter*>(target))
    {
      if(modParam->getModulationSource() == mc)
      {
        auto scope = Application::get().getUndoScope()->startTransaction("Remove Modulation Source");
        modParam->undoableSelectModSource(scope->getTransaction(), ModulateableParameter::NONE);
      }
      else
      {
        auto scope = Application::get().getUndoScope()->startTransaction("Set Modulation Source");
        modParam->undoableSelectModSource(scope->getTransaction(), mc);

        auto Editbuffer = Application::get().getPresetManager()->getEditBuffer();

        editBuffer->undoableSelectParameter(scope->getTransaction(), modParam);

        auto hwui = Application::get().getHWUI();
        auto layout = hwui->getPanelUnit().getEditPanel().getBoled().getLayout();

        if (auto modParamLayout = dynamic_cast<ModulateableParameterSelectLayout2*>(layout.get()))
        {

          modParamLayout->installMcAmountScreen();
          m_macroControlAssignmentStateMachine.setState(MacroControlAssignmentStates::Initial);
          return true;
        }

      }
    }
    return true;
  });

  m_macroControlAssignmentStateMachine.registerHandler(MacroControlAssignmentStates::SelectSource, [=]()
  {
    auto editBuffer = Application::get().getPresetManager()->getEditBuffer();
    auto p = editBuffer->getSelected();
    auto currentSource = choseHWBestSourceForMC(p->getID());
    editBuffer->undoableSelectParameter(currentSource);
    m_macroControlAssignmentStateMachine.setState(MacroControlAssignmentStates::Initial);
    return true;
  });

  setUsageMode(new PanelUnitParameterEditMode());

  Application::get().getWebSocketSession()->onConnectionEstablished(sigc::mem_fun(this, &PanelUnit::onBBBBConnected));
}

PanelUnit::~PanelUnit()
{
}

int PanelUnit::choseHWBestSourceForMC(int mcParamId) const
{
  if(auto mc = dynamic_cast<MacroControlParameter*>(Application::get().getPresetManager()->getEditBuffer()->findParameterByID(mcParamId)))
  {
    return mc->getUiSelectedHardwareSource();
  }

  return mcParamId;
}

void PanelUnit::init()
{
  for(auto &led : m_leds)
    led->init();

  m_editPanel.init();
}

void PanelUnit::onBBBBConnected()
{
  for(auto &led : m_leds)
    led->syncBBBB();
}

void PanelUnit::turnLedsOff()
{
  for(auto &led : m_leds)
    led->setState(TwoStateLED::OFF);
}

void PanelUnit::setupFocusAndMode()
{
  setupFocusAndMode(Application::get().getHWUI()->getFocusAndMode());
}

void PanelUnit::setupFocusAndMode(FocusAndMode focusAndMode)
{
  installUsageMode(focusAndMode);

  m_editPanel.setupFocusAndMode(focusAndMode);
  getUsageMode()->setupFocusAndMode(focusAndMode);
}

void PanelUnit::installUsageMode(FocusAndMode focusAndMode)
{
  switch(focusAndMode.focus)
  {
    case UIFocus::Presets:
    case UIFocus::Banks:
      setUsageMode(new PanelUnitPresetMode());
      break;

    case UIFocus::Sound:
    case UIFocus::Parameters:
    case UIFocus::Setup:
    case UIFocus::Unchanged:
      setUsageMode(new PanelUnitParameterEditMode());
      break;
  }
}

PanelUnit::tLed PanelUnit::getLED(int id)
{
  return m_leds[id];
}

void PanelUnit::onTimeout()
{
  m_editPanel.onTimeout();
}

EditPanel & PanelUnit::getEditPanel()
{
  return m_editPanel;
}

const EditPanel &PanelUnit::getEditPanel() const
{
  return m_editPanel;
}

bool PanelUnit::onButtonPressed(gint32 buttonID, ButtonModifiers modifiers, bool state)
{
  if(super::onButtonPressed(buttonID, modifiers, state))
  {
    return true;
  }

  return m_editPanel.onButtonPressed(buttonID, modifiers, state);
}

MacroControlAssignmentStateMachine &PanelUnit::getMacroControlAssignmentStateMachine()
{
  return m_macroControlAssignmentStateMachine;
}
