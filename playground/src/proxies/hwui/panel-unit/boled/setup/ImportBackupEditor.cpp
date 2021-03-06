#include <Application.h>
#include <glibmm/ustring.h>
#include <http/UndoScope.h>
#include <libundo/undo/Transaction.h>
#include <libundo/undo/TransactionCreationScope.h>
#include <presets/PresetManager.h>
#include <proxies/hwui/buttons.h>
#include <proxies/hwui/controls/Button.h>
#include <proxies/hwui/controls/ControlOwner.h>
#include <proxies/hwui/controls/Point.h>
#include <proxies/hwui/controls/Rect.h>
#include <proxies/hwui/HWUI.h>
#include <proxies/hwui/HWUIEnums.h>
#include <proxies/hwui/panel-unit/boled/BOLED.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/MultiLineLabel.h>
#include <proxies/hwui/panel-unit/boled/setup/ImportBackupEditor.h>
#include <proxies/hwui/panel-unit/boled/SplashLayout.h>
#include <proxies/hwui/panel-unit/EditPanel.h>
#include <proxies/hwui/panel-unit/PanelUnit.h>
#include <serialization/PresetManagerSerializer.h>
#include <xml/FileInStream.h>
#include <xml/XmlReader.h>
#include <xml/VersionAttribute.h>
#include <memory>
#include <thread>
#include "USBStickAvailableView.h"
#include "device-settings/DebugLevel.h"
#include <tools/FileTools.h>
#include <proxies/hwui/panel-unit/boled/file/FileDialogLayout.h>

static const Rect c_fullRightSidePosition(129, 16, 126, 48);

ImportBackupEditor::ImportBackupEditor () :
    ControlWithChildren (Rect (0, 0, 0, 0))
{
  if (USBStickAvailableView::usbIsReady())
  {
    addLabel("Attention! This action will delete all current banks!");
    addControl(new Button("OK", Button::getButtonPos(BUTTON_C).getMovedBy(Point(-128, -16))));
    addControl(new Button("Cancel", Button::getButtonPos(BUTTON_D).getMovedBy(Point(-128, -16))));
  }
  else
  {
    addControl(new MultiLineLabel("Please insert an USB-Stick and reload this page!"))->setPosition(Rect(3, 0, 119, 24));
    addControl(new Button("Back", Button::getButtonPos(BUTTON_D).getMovedBy(Point(-128, -16))));
  }
}

ImportBackupEditor::~ImportBackupEditor ()
{
}

void ImportBackupEditor::addLabel (const Glib::ustring &text)
{
  addControl (new MultiLineLabel (text))->setPosition (Rect (3, 0, 119, 24));
}

void ImportBackupEditor::setPosition (const Rect &)
{
  ControlWithChildren::setPosition (c_fullRightSidePosition);
}

bool ImportBackupEditor::onButton (int i, bool down, ButtonModifiers modifiers)
{
  if (down)
  {
    if (i == BUTTON_C && USBStickAvailableView::usbIsReady())
    {
      importBackup ();
      return true;
    }
    else if (i == BUTTON_D)
    {
      diveUp ();
      return true;
    }
  }
  return false;
}

Glib::ustring ImportBackupEditor::generateFileDialogCompliantNameFromPath(std::experimental::filesystem::directory_entry file)
{
  auto nameWithoutZipAtTheEnd = file.path().stem().string();
  auto parentPathToFile = file.path().parent_path().string();
  return parentPathToFile + "/" + nameWithoutZipAtTheEnd;
}

bool ImportBackupEditor::filterApplicableFileNames(std::experimental::filesystem::directory_entry term)
{
  auto fileName = term.path().filename().string();
  string endA = ".xml.zip";
  string endB = ".xml.tar.gz";
  return !(std::equal(endA.rbegin(), endA.rend(), fileName.rbegin()) ||  std::equal(endB.rbegin(), endB.rend(), fileName.rbegin()));
}

void ImportBackupEditor::importBackupFileFromPath(std::experimental::filesystem::directory_entry file)
{
  auto &app = Application::get();
  auto &boled = app.getHWUI()->getPanelUnit().getEditPanel().getBoled();

  if (file != std::experimental::filesystem::directory_entry())
  {

    auto path = generateFileDialogCompliantNameFromPath(file);

    FileInStream in(path, true);

    if (!in.eof())
    {
      auto scope = app.getUndoScope()->startTransaction("Import Presetmanager Backup");
      auto pm = app.getPresetManager();


      pm->undoableClear(scope->getTransaction());
      PresetManagerSerializer serializer(*pm.get());

      boled.resetOverlay();
      boled.setOverlay(new SplashLayout());
      SplashLayout::addStatus("Restoring Backup from File!");

      XmlReader reader(in, scope->getTransaction());
      reader.onFileVersionRead([=](int version)
      {
        if (version > VersionAttribute::getCurrentFileVersion ())
        {
          SplashLayout::setStatus("Unsupported File Version. The backup was created with a newer firmware. Please update your C15.");
          std::this_thread::sleep_for (2s);
          scope->getTransaction ()->rollBack ();
          return Reader::FileVersionCheckResult::Unsupported;
        }
        return Reader::FileVersionCheckResult::OK;
      });

      reader.read<PresetManagerSerializer>(std::ref(*pm.get()));
      SplashLayout::addStatus("Restore Complete!");
      std::this_thread::sleep_for(0.7s);
    }
  }
  boled.resetOverlay();
  Application::get().getHWUI()->getPanelUnit().setupFocusAndMode(
  { UIFocus::Presets, UIMode::Select });
}

void ImportBackupEditor::importBackup()
{
  Application::get().getHWUI()->getPanelUnit().getEditPanel().getBoled().reset(
          new FileDialogLayout(&ImportBackupEditor::filterApplicableFileNames,
                               &ImportBackupEditor::importBackupFileFromPath,
                               "Select Backup for Restore"));
}
