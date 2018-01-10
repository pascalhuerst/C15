#pragma once

#include "RenameLayout.h"
#include <libundo/undo/Transaction.h>

class Preset;

class RenamePresetLayout : public RenameLayout
{
  private:
    typedef RenameLayout super;

  public:
    typedef function<void (const Glib::ustring &)> tCommitCB;
    RenamePresetLayout (tCommitCB commitCB);
    void setTransactionToCancel (shared_ptr<UNDO::Transaction> transactionToCancelIfCancelled);

  protected:
    virtual void commit (const Glib::ustring &newName) override;
    virtual void cancel () override;
    virtual Glib::ustring getInitialText () const override;

  private:
    shared_ptr<Preset> m_currentPreset;
    shared_ptr<UNDO::Transaction> m_transactionToCancelIfCancelled;

    tCommitCB m_commitCB;
};
