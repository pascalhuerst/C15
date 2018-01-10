
#include <proxies/hwui/panel-unit/boled/preset-screens/controls/FileListControl.h>

FileListControl::FileListControl(FileTools::FileList&& files) :
    selectedItem(0), fileList(files), ControlWithChildren(Rect(0, 0, 150, 48))
{
  upperFile = addControl(new Label("", Rect(0, 0, 150, 16)));
  middleFile = addControl(new Label("", Rect(0, 16, 150, 16)));
  bottomFile = addControl(new Label("", Rect(0, 32, 150, 16)));

  middleFile->setHighlight(true);

  setSelection(0);
}

FileListControl::~FileListControl()
{

}

void FileListControl::setSelection(int index)
{
  upperFile->setText(getFileNameAtIndex(index - 1));
  middleFile->setText(getFileNameAtIndex(index));
  bottomFile->setText(getFileNameAtIndex(index + 1));

  selectedItem = index;
}

std::string FileListControl::getFileNameAtIndex(int index)
{
  if (index >= 0 && index < fileList.size())
    return fileList[index].path().filename().string();
  return "";
}

void FileListControl::changeSelection(int change)
{
  auto newIndex = selectedItem + change;

  if (newIndex < 0)
    setSelection(0);
  else if (newIndex >= fileList.size())
    setSelection((int) fileList.size() - 1);
  else
    setSelection(newIndex);
}

fs::directory_entry FileListControl::getSelection() const
{
  return fileList.at(selectedItem);
}

int FileListControl::getSelectedIndex() const
{
  return selectedItem;
}
