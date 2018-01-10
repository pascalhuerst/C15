#include <Application.h>
#include <http/UndoScope.h>
#include <presets/Preset.h>
#include <presets/PresetBank.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <proxies/hwui/HWUI.h>
#include <proxies/hwui/HWUIEnums.h>
#include <proxies/hwui/panel-unit/boled/preset-screens/RenamePresetLayout.h>

RenamePresetLayout::RenamePresetLayout (tCommitCB commitCB) :
    super (),
    m_commitCB (commitCB)

{
  if (auto bank = Application::get ().getPresetManager ()->getSelectedBank ())
  {
    const auto &uuid = bank->getSelectedPreset ();
    m_currentPreset = bank->getPreset (uuid);
  }
}

void RenamePresetLayout::setTransactionToCancel (shared_ptr<UNDO::Transaction> transactionToCancelIfCancelled)
{
  m_transactionToCancelIfCancelled = transactionToCancelIfCancelled;
}

void RenamePresetLayout::commit (const Glib::ustring &newName)
{
  if (m_commitCB)
    m_commitCB (newName);

}

void RenamePresetLayout::cancel ()
{
  RenameLayout::cancel ();

  m_commitCB = {};

  auto scope = Application::get ().getUndoScope ();

  while (m_transactionToCancelIfCancelled && scope->canUndo ())
  {
    auto tip = scope->getUndoTransaction ();
    bool isResponsible = tip == m_transactionToCancelIfCancelled;
    scope->undoAndHushUp ();

    if (isResponsible)
      break;
  }
}

Glib::ustring RenamePresetLayout::getInitialText () const
{
  if (m_currentPreset)
    return m_currentPreset->getName ();

  return "";
}

