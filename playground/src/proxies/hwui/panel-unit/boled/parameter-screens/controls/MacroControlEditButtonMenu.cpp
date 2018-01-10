#include <Application.h>
#include <parameters/MacroControlParameter.h>
#include <presets/EditBuffer.h>
#include <presets/PresetManager.h>
#include <proxies/hwui/HWUI.h>
#include <proxies/hwui/panel-unit/boled/BOLED.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/MacroControlEditButtonMenu.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/EditMCInfoLayout.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/RenameMCLayout.h>
#include <proxies/hwui/panel-unit/EditPanel.h>
#include <proxies/hwui/panel-unit/PanelUnit.h>

int MacroControlEditButtonMenu::s_lastAction = 0;

MacroControlEditButtonMenu::MacroControlEditButtonMenu (const Rect &rect) :
    super (rect)
{
  addButton ("Smoothing", bind (&MacroControlEditButtonMenu::smoothing, this));
  addButton ("Rename", bind (&MacroControlEditButtonMenu::rename, this));
  addButton ("Mod Reset", bind (&MacroControlEditButtonMenu::reset, this));

  selectButton(s_lastAction);
}

MacroControlEditButtonMenu::~MacroControlEditButtonMenu ()
{
}

void MacroControlEditButtonMenu::selectButton (size_t i)
{
  super::selectButton (i);
  s_lastAction = i;
}

void MacroControlEditButtonMenu::rename ()
{
  Application::get ().getHWUI ()->getPanelUnit ().getEditPanel ().getBoled ().setOverlay (new RenameMCLayout ());
}

void MacroControlEditButtonMenu::smoothing ()
{
  auto eb = Application::get ().getPresetManager()->getEditBuffer();
  const auto currentID = eb->getSelected()->getID();
  const auto diffBetweenMacroControlIDAndItsSmoothing = 81;
  const auto smoothingID = currentID + diffBetweenMacroControlIDAndItsSmoothing;
  eb->undoableSelectParameter(smoothingID);
}

void MacroControlEditButtonMenu::editInfo ()
{
  Application::get ().getHWUI ()->getPanelUnit ().getEditPanel ().getBoled ().setOverlay (new EditMCInfoLayout ());
}

void MacroControlEditButtonMenu::reset ()
{
  if (auto p = dynamic_cast<MacroControlParameter*> (Application::get ().getPresetManager ()->getEditBuffer ()->getSelected ()))
  {
    p->undoableResetConnectionsToTargets ();
  }
}
