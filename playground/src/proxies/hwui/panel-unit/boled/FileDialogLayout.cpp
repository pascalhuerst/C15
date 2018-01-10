#include <proxies/hwui/panel-unit/boled/FileDialogLayout.h>
#include <device-settings/DebugLevel.h>

FileDialogLayout::FileDialogLayout(FileTools::FileList&& files, std::function<void(std::experimental::filesystem::directory_entry)> cb) :
    DFBLayout(Application::get().getHWUI()->getPanelUnit().getEditPanel().getBoled()), commitFunction(cb)
{
  fileCount = files.size();
  fileList = addControl(new FileListControl(std::move(files)));
  positionLabel = addControl(new Label("", Rect(196, 0, 40, 12)));
  updateLabels();
}

FileDialogLayout::~FileDialogLayout()
{

}

bool FileDialogLayout::onButton(int i, bool down, ButtonModifiers modifiers)
{
  if (down)
  {
    if (i == BUTTON_ENTER)
    {
      try
      {
        commitFunction(fileList->getSelection());
      }
      catch (...)
      {
        DebugLevel::error(__FILE__, __LINE__);
      }
      return true;
    }
  }
  return DFBLayout::onButton(i, down, modifiers);
}

bool FileDialogLayout::onRotary(int inc, ButtonModifiers modifiers)
{
  fileList->changeSelection(inc);
  updateLabels();
  return true;
}

void FileDialogLayout::updateLabels()
{
  positionLabel->setText("[" + std::to_string(fileList->getSelectedIndex() + 1) + "/" + std::to_string(fileCount) + "]");
}
