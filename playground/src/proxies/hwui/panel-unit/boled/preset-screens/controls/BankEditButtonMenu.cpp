#include <Application.h>
#include <clipboard/Clipboard.h>
#include <libundo/undo/Scope.h>
#include <presets/PresetBank.h>
#include <presets/PresetManager.h>
#include <proxies/hwui/HWUI.h>
#include <proxies/hwui/panel-unit/boled/BOLED.h>
#include <proxies/hwui/panel-unit/boled/preset-screens/controls/BankEditButtonMenu.h>
#include <proxies/hwui/panel-unit/boled/preset-screens/RenameBankLayout.h>
#include <proxies/hwui/panel-unit/EditPanel.h>
#include <proxies/hwui/panel-unit/PanelUnit.h>
#include <tools/FileTools.h>
#include <device-settings/DebugLevel.h>
#include <serialization/PresetBankSerializer.h>
#include <xml/FileOutStream.h>
#include <xml/XmlWriter.h>
#include <proxies/hwui/panel-unit/boled/SplashLayout.h>
#include <proxies/hwui/panel-unit/boled/setup/USBStickAvailableView.h>
#include <tools/SpawnCommandLine.h>
#include <proxies/hwui/panel-unit/boled/FileDialogLayout.h>
#include <xml/FileInStream.h>
#include <xml/VersionAttribute.h>

static int s_lastSelectedButton = 0;

BankEditButtonMenu::BankEditButtonMenu(const Rect &rect) :
    super(rect)
{
  Application::get().getClipboard()->onClipboardChanged(mem_fun(this, &BankEditButtonMenu::rebuildMenu));
  Glib::MainContext::get_default()->signal_timeout().connect_seconds(mem_fun(this, &BankEditButtonMenu::interruptToRebuildMenu), 5);
}

BankEditButtonMenu::~BankEditButtonMenu()
{
}

bool BankEditButtonMenu::interruptToRebuildMenu()
{
  rebuildMenu();
  return true;
}

void BankEditButtonMenu::rebuildMenu()
{
  clear();
  clearActions();

  addButton("New", bind(&BankEditButtonMenu::newBank, this));

  if (USBStickAvailableView::usbIsReady())
  {
    addButton("Import Bank", bind(&BankEditButtonMenu::importBank, this));
    addButton("Export Bank", bind(&BankEditButtonMenu::exportBank, this));
  }

  addButton("Rename", bind(&BankEditButtonMenu::renameBank, this));
  addButton("Copy", bind(&BankEditButtonMenu::copyBank, this));

  if(Application::get().getClipboard()->hasContent())
  {
    addButton("Paste", bind(&BankEditButtonMenu::pasteBank, this));
  }

  addButton("Delete", bind(&BankEditButtonMenu::deleteBank, this));
  addButton("Move Left", bind(&BankEditButtonMenu::moveLeft, this));
  addButton("Move Right", bind(&BankEditButtonMenu::moveRight, this));

  selectButton(s_lastSelectedButton);
}

void BankEditButtonMenu::selectButton(size_t i)
{
  s_lastSelectedButton = i;
  super::selectButton(i);
}

void BankEditButtonMenu::newBank()
{
  auto pm = Application::get().getPresetManager();
  auto scope = pm->getUndoScope().startTransaction("New bank");
  auto transaction = scope->getTransaction();
  auto newBank = pm->addBank(transaction, true);
  newBank->assignDefaultPosition();

  Application::get().getHWUI()->undoableSetFocusAndMode(transaction, FocusAndMode(UIFocus::Presets, UIMode::Select));

  auto layout = new RenameBankLayout(transaction);
  Application::get().getHWUI()->getPanelUnit().getEditPanel().getBoled().setOverlay(layout);
}

BankEditButtonMenu::FileInfos BankEditButtonMenu::extractFileInfos(std::experimental::filesystem::directory_entry file)
{
  FileInfos infos;
  infos.filePath = file.path().string();
  infos.fileName = file.path().filename().string();
  auto lastModified = std::experimental::filesystem::last_write_time(file);
  infos.millisecondsFromEpoch = std::chrono::duration_cast<std::chrono::milliseconds>(lastModified.time_since_epoch()).count();
  return infos;
}

bool BankEditButtonMenu::applicableBackupFilesFilter(std::experimental::filesystem::directory_entry term)
{
  auto fileName = term.path().filename().string();
  return fileName.find(".xml") == Glib::ustring::npos ? true : fileName.find(".xml.zip") != Glib::ustring::npos;
}

void BankEditButtonMenu::importBankFromPath(std::experimental::filesystem::directory_entry file)
{
  auto hwui = Application::get().getHWUI();
  auto fileInfos = extractFileInfos(file);

  hwui->getPanelUnit().getEditPanel().getBoled().setOverlay(new SplashLayout());

  FileInStream stream(fileInfos.filePath, false);
  SplashLayout::addStatus("Importing " + fileInfos.fileName);
  Application::get().getPresetManager()->importBank(stream, "", "", fileInfos.fileName, std::to_string(fileInfos.millisecondsFromEpoch));

  hwui->getPanelUnit().getEditPanel().getBoled().resetOverlay();
}

void BankEditButtonMenu::importBank()
{
  auto matchedFiles = FileTools::getListOfFilesThatMatchFilter("/mnt/usb-stick/",
      &BankEditButtonMenu::applicableBackupFilesFilter);

  Application::get().getHWUI()->getPanelUnit().getEditPanel().getBoled().setOverlay(
      new FileDialogLayout(std::move(matchedFiles), &BankEditButtonMenu::importBankFromPath));
}

Glib::ustring BankEditButtonMenu::sanitizeExportBankname(std::string str)
{
  str.erase(std::remove_if(str.begin(), str.end(), [](unsigned char c)->bool
  {
    return !std::isalnum(c);
  }), str.end());

  if (str.empty())
  {
    str = "NoPrintAbleSymbolsInBankName";
  }
  return str;
}

Glib::ustring BankEditButtonMenu::createValidOutputPath(const Glib::ustring& bankName)
{
  auto fileName = FileTools::findSuitableFileName(bankName, "/mnt/usb-stick/", 0);
  return "/mnt/usb-stick/" + fileName + ".xml";
}

void BankEditButtonMenu::exportBank()
{
  auto hwui = Application::get().getHWUI();
  auto selBank = Application::get().getPresetManager()->getSelectedBank();
  auto bankName(selBank->getName(true));

  bankName = sanitizeExportBankname(bankName.c_str());
  auto outPath = createValidOutputPath(bankName);

  hwui->getPanelUnit().getEditPanel().getBoled().setOverlay(new SplashLayout());
  writeSelectedBankToFile(selBank, outPath);
  hwui->getPanelUnit().getEditPanel().getBoled().resetOverlay();
}

void BankEditButtonMenu::writeSelectedBankToFile(PresetManager::tBankPtr selBank, const std::string& outFile)
{
  SplashLayout::addStatus("Exporting " + selBank->getName(true));
  PresetBankSerializer serializer(selBank, false);
  XmlWriter writer(std::make_shared < FileOutStream > (outFile, false));
  serializer.write(writer, VersionAttribute::get());
}

void BankEditButtonMenu::renameBank()
{
  auto layout = new RenameBankLayout();
  Application::get().getHWUI()->getPanelUnit().getEditPanel().getBoled().setOverlay(layout);
}

void BankEditButtonMenu::copyBank()
{
  auto pm = Application::get().getPresetManager();

  if(auto bank = pm->getSelectedBank())
  {
    Application::get().getClipboard()->copyBank(bank->getUuid());
  }
}

void BankEditButtonMenu::pasteBank()
{
  auto pm = Application::get().getPresetManager();

  if(auto bank = pm->getSelectedBank())
  {
    Application::get().getClipboard()->pasteOnBank(bank->getUuid());
  }
}

void BankEditButtonMenu::deleteBank()
{
  auto pm = Application::get().getPresetManager();

  if(auto bank = pm->getSelectedBank())
  {
    auto scope = bank->getUndoScope().startTransaction("Delete bank '%0'", bank->getName(true));
    pm->undoableDeleteSelectedBank(scope->getTransaction());
  }
}

void BankEditButtonMenu::moveLeft()
{
  auto pm = Application::get().getPresetManager();

  if(auto bank = pm->getSelectedBank())
  {
    auto scope = bank->getUndoScope().startTransaction("Move bank '%0' up", bank->getName(true));
    pm->undoableMoveBankBy(scope->getTransaction(), bank->getUuid(), -1);
  }
}

void BankEditButtonMenu::moveRight()
{
  auto pm = Application::get().getPresetManager();

  if(auto bank = pm->getSelectedBank())
  {
    auto scope = bank->getUndoScope().startTransaction("Move bank '%0' down", bank->getName(true));
    pm->undoableMoveBankBy(scope->getTransaction(), bank->getUuid(), 1);
  }
}

