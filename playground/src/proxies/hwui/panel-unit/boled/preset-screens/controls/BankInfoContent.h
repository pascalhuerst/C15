#pragma once

#include <presets/PresetBank.h>
#include <proxies/hwui/controls/ControlWithChildren.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/Scrollable.h>

class MultiLineLabel;
class Label;
class PresetBank;

class BankInfoContent : public ControlWithChildren, public Scrollable
{
  private:
    typedef ControlWithChildren super;

  public:
    BankInfoContent ();
    virtual ~BankInfoContent ();

    void setPosition (const Rect &rect) override;
    const Rect &getPosition () const override;
    void setDirty () override;

  private:
    void onBankSelectionChanged (shared_ptr<PresetBank> bank);
    void onBankChanged (shared_ptr<PresetBank> bank);

    void fixLayout();
    bool fillFromBank (const PresetBank *bank);
    bool fillDefaults ();

    sigc::connection m_bankConnection;

    Label *m_nameLabel;
    Label *m_commentLabel;
    Label *m_sizeLabel;
    Label *m_dateOfLastChangeLabel;
    Label *m_dateOfImportFileLabel;
    Label *m_nameOfImportFileLabel;

    MultiLineLabel *m_name;
    MultiLineLabel *m_comment;
    Label *m_size;
    Label *m_dateOfLastChange;
    Label *m_dateOfImportFile;
    Label *m_nameOfImportFile;
};

