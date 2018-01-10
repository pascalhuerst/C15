#pragma once

#include "proxies/hwui/controls/ButtonMenu.h"
#include <experimental/filesystem>

class PresetBank;

class BankEditButtonMenu : public ButtonMenu
{
  private:
    typedef ButtonMenu super;

    struct FileInfos
    {
        std::string fileName;
        std::string filePath;
        long millisecondsFromEpoch;
    };

  public:
    BankEditButtonMenu (const Rect &rect);
    virtual ~BankEditButtonMenu ();

    void selectButton (size_t i) override;
    void rebuildMenu();

  private:
    BankEditButtonMenu (const BankEditButtonMenu& other);
    BankEditButtonMenu& operator= (const BankEditButtonMenu&);

    void newBank ();
    void importBank();
    void renameBank ();
    void exportBank();
    void copyBank ();
    void pasteBank ();
    void deleteBank ();
    void moveLeft ();
    void moveRight ();

    Glib::ustring sanitizeExportBankname(std::string str);
    Glib::ustring createValidOutputPath(const Glib::ustring& bankName);

    void writeSelectedBankToFile(PresetManager::tBankPtr selBank, const std::string& outFile);
    bool interruptToRebuildMenu();
    static FileInfos extractFileInfos(std::experimental::filesystem::directory_entry file);
    static void importBankFromPath(std::experimental::filesystem::directory_entry file);
    static bool applicableBackupFilesFilter(std::experimental::filesystem::directory_entry term);
};

