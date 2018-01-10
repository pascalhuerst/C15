#pragma once

#include <proxies/hwui/DFBLayout.h>
#include <proxies/hwui/panel-unit/boled/preset-screens/controls/FileListControl.h>
#include <tools/FileTools.h>
#include <Application.h>
#include <proxies/hwui/HWUI.h>
#include <proxies/hwui/controls/Label.h>
#include <experimental/filesystem>

class FileDialogLayout : public DFBLayout
{
  private:
    typedef std::function<void(std::experimental::filesystem::directory_entry)> tCallBackFunction;

 public:
    FileDialogLayout(FileTools::FileList&& files, tCallBackFunction cb);
    virtual ~FileDialogLayout();

    bool onButton(int i, bool down, ButtonModifiers modifiers) override;
    bool onRotary(int inc, ButtonModifiers modifiers) override;

 private:
    void updateLabels();

    int fileCount = 0;
    FileListControl* fileList = nullptr;
    Label* positionLabel = nullptr;
    tCallBackFunction commitFunction;
};
