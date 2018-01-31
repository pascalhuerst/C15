#include "BankActions.h"

#include <serialization/PresetBankSerializer.h>
#include "PresetManager.h"
#include "PresetBank.h"
#include "EditBuffer.h"
#include "http/SoupOutStream.h"
#include <http/HTTPRequest.h>
#include "xml/XmlWriter.h"
#include <xml/MemoryInStream.h>
#include <xml/XmlReader.h>
#include <device-settings/AutoLoadSelectedPreset.h>
#include <device-settings/Settings.h>
#include <device-info/DateTimeInfo.h>
#include <Application.h>
#include <tools/PerformanceTimer.h>
#include <xml/VersionAttribute.h>
#include <boost/algorithm/string.hpp>
#include <algorithm>

BankActions::BankActions(PresetManager &presetManager) :
    RPCActionManager("/presets/banks/"),
    m_presetManager(presetManager)
{
  addAction("drop-presets-above", [&] (shared_ptr<NetworkRequest> request)
  {
    UNDO::Scope::tTransactionScopePtr scope = m_presetManager.getUndoScope().startTransaction ("Drop Presets");
    auto transaction = scope->getTransaction();

    Glib::ustring csv = request->get ("presets");
    Glib::ustring anchorUUID = request->get ("anchor");

    vector<string> strs;
    boost::split(strs, csv, boost::is_any_of(","));

    dropPresets(transaction, anchorUUID, 0, csv);
  });

  addAction("drop-presets-below", [&] (shared_ptr<NetworkRequest> request)
  {
    UNDO::Scope::tTransactionScopePtr scope = m_presetManager.getUndoScope().startTransaction ("Drop Presets");
    auto transaction = scope->getTransaction();

    Glib::ustring csv = request->get ("presets");
    Glib::ustring anchorUUID = request->get ("anchor");

    dropPresets(transaction, anchorUUID, 1, csv);
  });

  addAction("drop-presets-to", [&] (shared_ptr<NetworkRequest> request)
  {
    UNDO::Scope::tTransactionScopePtr scope = m_presetManager.getUndoScope().startTransaction ("Drop Presets");
    auto transaction = scope->getTransaction();

    Glib::ustring csv = request->get ("presets");
    Glib::ustring anchorUUID = request->get ("anchor");

    dropPresets(transaction, anchorUUID, 1, csv);

    if(auto anchor = presetManager.findPreset(anchorUUID))
    {
      anchor->getBank()->undoableDeletePreset(transaction, anchorUUID);
    }
  });

  addAction("rename-preset",
      [&] (shared_ptr<NetworkRequest> request)
      {
        Glib::ustring uuid = request->get ("uuid");
        Glib::ustring newName = request->get ("name");

        if (auto p = m_presetManager.findPreset (uuid))
        {
          UNDO::Scope::tTransactionScopePtr scope = m_presetManager.getUndoScope().startTransaction ("Rename preset '%0' to '%1'", p->getName(), newName);
          p->undoableSetName (scope->getTransaction(), newName);
          p->undoableSelect (scope->getTransaction());
          m_presetManager.getEditBuffer()->undoableUpdateLoadedPresetInfo (scope->getTransaction());
        }
      });

  addAction("move-preset-above", [&] (shared_ptr<NetworkRequest> request) mutable
  {
    Glib::ustring presetToMove = request->get ("presetToMove");
    Glib::ustring presetAnchor = request->get ("anchor");

    tBankPtr srcBank = m_presetManager.findBankWithPreset (presetToMove);
    tBankPtr tgtBank = m_presetManager.findBankWithPreset (presetAnchor);

    if (srcBank && tgtBank)
    {
      UNDO::Scope::tTransactionScopePtr scope = m_presetManager.getUndoScope().startTransaction ("Move preset");
      auto transaction = scope->getTransaction();

      tPresetPtr preset = srcBank->undoableExpropriatePreset (transaction, presetToMove);
      size_t anchorPos = tgtBank->getPresetPosition (presetAnchor);
      tgtBank->undoableAdoptPreset (transaction, anchorPos, preset);
      preset->undoableSelect (transaction);
      tgtBank->undoableSelect (transaction);
    }
  });

  addAction("move-preset-below", [&] (shared_ptr<NetworkRequest> request)
  {
    Glib::ustring presetToMove = request->get ("presetToMove");
    Glib::ustring presetAnchor = request->get ("anchor");

    tBankPtr srcBank = m_presetManager.findBankWithPreset (presetToMove);
    tBankPtr tgtBank = m_presetManager.findBankWithPreset (presetAnchor);

      if(presetToMove != presetAnchor) {
        if (srcBank && tgtBank)
        {
          UNDO::Scope::tTransactionScopePtr scope = m_presetManager.getUndoScope().startTransaction ("Move preset");
          auto transaction = scope->getTransaction();
          tPresetPtr preset = srcBank->undoableExpropriatePreset (transaction, presetToMove);
          size_t anchorPos = tgtBank->getPresetPosition (presetAnchor) + 1;
          tgtBank->undoableAdoptPreset(transaction, anchorPos, preset);
          preset->undoableSelect (transaction);
          tgtBank->undoableSelect (transaction);
        }
      }
  });
  
  addAction("move-preset-to", [&] (shared_ptr<NetworkRequest> request)
  {
    Glib::ustring presetToOverwrite = request->get ("presetToOverwrite");
    Glib::ustring overwriteWith = request->get ("overwriteWith");

    tBankPtr tgtBank = m_presetManager.findBankWithPreset (presetToOverwrite);
    tBankPtr srcBank = m_presetManager.findBankWithPreset (overwriteWith);

    if (srcBank == tgtBank && srcBank)
    {
      tPresetPtr srcPreset = srcBank->getPreset (overwriteWith);
      tPresetPtr tgtPreset = tgtBank->getPreset (presetToOverwrite);

      if (srcPreset != tgtPreset)
      {
        UNDO::Scope::tTransactionScopePtr scope = m_presetManager.getUndoScope().startTransaction ("Overwrite preset");
        auto transaction = scope->getTransaction();

        auto preset = srcBank->undoableExpropriatePreset(transaction, overwriteWith);
        auto pos = tgtBank->getPresetPosition(presetToOverwrite);
        tgtBank->undoableDeletePreset(transaction, presetToOverwrite);
        tgtBank->undoableAdoptPreset(transaction, pos, preset);
        tgtPreset->undoableSelect (transaction);
        tgtBank->undoableSelect (transaction);
      }
    }
  });

  addAction("overwrite-preset", [&] (shared_ptr<NetworkRequest> request)
  {
    Glib::ustring presetToOverwrite = request->get ("presetToOverwrite");
    Glib::ustring overwriteWith = request->get ("overwriteWith");

    if(presetToOverwrite.empty() && overwriteWith.empty())
    {
      if(auto p = presetManager.getSelectedPreset())
      {
        UNDO::Scope::tTransactionScopePtr scope = m_presetManager.getUndoScope().startTransaction ("Overwrite preset");
        auto transaction = scope->getTransaction();
        p->copyFrom(transaction, presetManager.getEditBuffer().get(), true);
      }
    }
    else
    {
      auto srcPreset = m_presetManager.findPreset (overwriteWith);
      auto tgtPreset = m_presetManager.findPreset (presetToOverwrite);

      if (srcPreset != tgtPreset)
      {
        UNDO::Scope::tTransactionScopePtr scope = m_presetManager.getUndoScope().startTransaction ("Overwrite preset '%0' with '%1'",
            tgtPreset->getName(), srcPreset->getName());
        tgtPreset->copyFrom (scope->getTransaction(), srcPreset.get(), true);
        tgtPreset->setAutoGeneratedAttributes ();
        tgtPreset->undoableSelect (scope->getTransaction());
      }
    }
  });

  addAction("copy-preset-above",
      [&] (shared_ptr<NetworkRequest> request)
      {
        Glib::ustring presetToMove = request->get ("presetToCopy");
        Glib::ustring presetAnchor = request->get ("anchor");

        tBankPtr srcBank = m_presetManager.findBankWithPreset (presetToMove);
        tBankPtr tgtBank = m_presetManager.findBankWithPreset (presetAnchor);

        if (srcBank && tgtBank)
        {
          tPresetPtr srcPreset = srcBank->getPreset (presetToMove);
          g_assert (srcPreset);

          size_t anchorPos = tgtBank->getPresetPosition (presetAnchor);

          UNDO::Scope::tTransactionScopePtr scope = m_presetManager.getUndoScope().startTransaction ("Copy preset '%0' to bank '%1' position %2",
              srcPreset->getName(), tgtBank->getName(true), anchorPos + 1);
          auto transaction = scope->getTransaction();
          tgtBank->undoableInsertPreset (transaction, anchorPos);
          tgtBank->undoableOverwritePreset (transaction, anchorPos, srcPreset);
          tgtBank->getPreset (anchorPos)->undoableSelect (transaction);
          tgtBank->undoableSelect (transaction);
        }
      });

  addAction("copy-preset-below",
      [&] (shared_ptr<NetworkRequest> request)
      {
        Glib::ustring presetToMove = request->get ("presetToCopy");
        Glib::ustring presetAnchor = request->get ("anchor");

        tBankPtr srcBank = m_presetManager.findBankWithPreset (presetToMove);
        tBankPtr tgtBank = m_presetManager.findBankWithPreset (presetAnchor);

        if (srcBank && tgtBank)
        {
          tPresetPtr srcPreset = srcBank->getPreset (presetToMove);
          g_assert (srcPreset);

          size_t anchorPos = tgtBank->getPresetPosition (presetAnchor) + 1;

          UNDO::Scope::tTransactionScopePtr scope = m_presetManager.getUndoScope().startTransaction ("Copy preset '%0' to bank '%1' position %2",
              srcPreset->getName(), tgtBank->getName(true), anchorPos + 1);
          auto transaction = scope->getTransaction();
          tgtBank->undoableInsertPreset (transaction, anchorPos);
          tgtBank->undoableOverwritePreset (transaction, anchorPos, srcPreset);
          tgtBank->getPreset (anchorPos)->undoableSelect (transaction);
          tgtBank->undoableSelect (transaction);
        }
      });

  addAction("insert-editbuffer-above",
      [&] (shared_ptr<NetworkRequest> request)
      {
        Glib::ustring presetAnchor = request->get ("anchor");

        tBankPtr tgtBank = m_presetManager.findBankWithPreset (presetAnchor);

        if (tgtBank)
        {
          size_t anchorPos = tgtBank->getPresetPosition (presetAnchor);
          UNDO::Scope::tTransactionScopePtr scope = m_presetManager.getUndoScope().startTransaction ("Save new preset in bank '%0' at position %1",
              tgtBank->getName(true), anchorPos + 1);
          auto transaction = scope->getTransaction();
          tgtBank->undoableInsertPreset (transaction, anchorPos);
          tgtBank->undoableOverwritePreset (transaction, anchorPos, m_presetManager.getEditBuffer());
          tgtBank->getPreset (anchorPos)->undoableSelect (transaction);
          tgtBank->undoableSelect (transaction);
        }
      });

  addAction("insert-editbuffer-below",
      [&] (shared_ptr<NetworkRequest> request)
      {
        Glib::ustring presetAnchor = request->get ("anchor");

        tBankPtr tgtBank = m_presetManager.findBankWithPreset (presetAnchor);

        if (tgtBank)
        {
          size_t anchorPos = tgtBank->getPresetPosition (presetAnchor) + 1;
          UNDO::Scope::tTransactionScopePtr scope = m_presetManager.getUndoScope().startTransaction ("Save new preset in bank '%0' at position %1",
              tgtBank->getName(true), anchorPos + 1);
          auto transaction = scope->getTransaction();
          tgtBank->undoableInsertPreset (transaction, anchorPos);
          tgtBank->undoableOverwritePreset (transaction, anchorPos, m_presetManager.getEditBuffer());
          tgtBank->getPreset (anchorPos)->undoableSelect (transaction);
          tgtBank->undoableSelect (transaction);
        }
      });

  addAction("overwrite-preset-with-editbuffer",
      [&] (shared_ptr<NetworkRequest> request)
      {
        Glib::ustring presetToOverwrite = request->get ("presetToOverwrite");

        if (tBankPtr tgtBank = m_presetManager.findBankWithPreset (presetToOverwrite))
        {
          if(auto tgtPreset = tgtBank->getPreset (presetToOverwrite))
          {
            size_t anchorPos = tgtBank->getPresetPosition (presetToOverwrite);
            UNDO::Scope::tTransactionScopePtr scope = m_presetManager.getUndoScope().startTransaction ("Overwrite preset '%0'", tgtPreset->getName());
            auto transaction = scope->getTransaction();
            tgtBank->undoableOverwritePreset (transaction, anchorPos, m_presetManager.getEditBuffer());
            tgtBank->getPreset (anchorPos)->undoableSelect (transaction);
            tgtBank->undoableSelect (transaction);
          }
        }
      });

  addAction("append-preset", [&] (shared_ptr<NetworkRequest> request) mutable
  {
    if (tBankPtr bank = m_presetManager.getSelectedBank())
    {
      auto uuid = request->get ("uuid");
      auto newName = presetManager.createPresetNameBasedOn (m_presetManager.getEditBuffer()->getName());

      auto scope = m_presetManager.getUndoScope().startTransaction ("Append preset");
      auto transaction = scope->getTransaction();

      int desiredPresetPos = bank->getNumPresets();
      bank->undoableInsertPreset (transaction, desiredPresetPos);
      bank->undoableOverwritePreset (transaction, desiredPresetPos, m_presetManager.getEditBuffer());

      auto newPreset = bank->getPreset (desiredPresetPos);

      if(!uuid.empty())
      newPreset->undoableSetUuid(transaction, uuid);

      bank->undoableSelectPreset (transaction, newPreset->getUuid());
      newPreset->undoableSetName(transaction, newName);
      m_presetManager.undoableSelectBank (transaction, bank->getUuid());
    }
  });

  addAction("append-preset-to-bank", [&] (shared_ptr<NetworkRequest> request) mutable
  {
    Glib::ustring bankUuid = request->get ("bank-uuid");

    if (tBankPtr tgtBank = m_presetManager.findBank (bankUuid))
    {
      Glib::ustring presetUuid = request->get ("preset-uuid");

      if (tBankPtr srcBank = m_presetManager.findBankWithPreset (presetUuid))
      {
        if (tPresetPtr srcPreset = srcBank->getPreset (presetUuid))
        {
          UNDO::Scope::tTransactionScopePtr scope = m_presetManager.getUndoScope().startTransaction ("Append preset '%0' to bank '%1'",
              srcPreset->getName(), tgtBank->getName(true));
          UNDO::Scope::tTransactionPtr transaction = scope->getTransaction();

          int desiredPresetPos = tgtBank->getNumPresets();
          tgtBank->undoableInsertPreset (transaction, desiredPresetPos);
          tgtBank->undoableOverwritePreset (transaction, desiredPresetPos, srcPreset);
          tgtBank->undoableSelectPreset (transaction, tgtBank->getPreset (desiredPresetPos)->getUuid());
          tgtBank->undoableSelect (transaction);
        }
      }
    }
  });

  addAction("append-editbuffer-to-bank",
      [&] (shared_ptr<NetworkRequest> request) mutable
      {
        Glib::ustring bankUuid = request->get ("bank-uuid");

        if (tBankPtr tgtBank = m_presetManager.findBank (bankUuid))
        {
          UNDO::Scope::tTransactionScopePtr scope = m_presetManager.getUndoScope().startTransaction ("Append preset to bank '%0'", tgtBank->getName(true));
          UNDO::Scope::tTransactionPtr transaction = scope->getTransaction();

          int desiredPresetPos = tgtBank->getNumPresets();
          tgtBank->undoableInsertPreset (transaction, desiredPresetPos);
          tgtBank->undoableOverwritePreset (transaction, desiredPresetPos, m_presetManager.getEditBuffer());
          tgtBank->undoableSelectPreset (transaction, tgtBank->getPreset (desiredPresetPos)->getUuid());
          tgtBank->undoableSelect (transaction);
        }
      });

  addAction("set-order-number", [&] (shared_ptr<NetworkRequest> request) mutable
  {
      auto uuid = request->get("uuid");
      if(auto bank = m_presetManager.findBank(uuid))
      {
        int numBanks = static_cast<int>(m_presetManager.getNumBanks());
        int newPos = stoi(request->get("order-number"));
        newPos = std::max(newPos, 0);
        newPos = std::min(newPos, numBanks);

        auto oldBankPos = m_presetManager.calcOrderNumber(bank.get());

        auto scope = m_presetManager.getUndoScope().startTransaction("Changed Bank: %0 Order", bank->getName(true));
        auto movedPos = newPos - oldBankPos;
        if(movedPos != 0)
          m_presetManager.undoableMoveBankBy(scope->getTransaction(), bank->getUuid(), movedPos);
      }
  });

  addAction("insert-preset", [&] (shared_ptr<NetworkRequest> request) mutable
  {
    if (tBankPtr bank = m_presetManager.getSelectedBank())
    {
      auto uuid = request->get ("uuid");
      auto newName = presetManager.createPresetNameBasedOn (m_presetManager.getEditBuffer()->getName());

      auto scope = m_presetManager.getUndoScope().startTransaction ("Insert preset");
      auto transaction = scope->getTransaction();

      int selPreset = bank->getPresetPosition (bank->getSelectedPreset());
      int desiredPresetPos = selPreset + 1;
      bank->undoableInsertPreset (transaction, desiredPresetPos);
      bank->undoableOverwritePreset (transaction, desiredPresetPos, m_presetManager.getEditBuffer());

      auto newPreset = bank->getPreset (desiredPresetPos);

      if(!uuid.empty())
      newPreset->undoableSetUuid(transaction, uuid);

      bank->undoableSelectPreset (transaction, newPreset->getUuid());
      newPreset->undoableSetName(transaction, newName);
      m_presetManager.undoableSelectBank (transaction, bank->getUuid());
    }
  });

  addAction("insert-named-preset", [&] (shared_ptr<NetworkRequest> request) mutable
  {
    if (tBankPtr bank = m_presetManager.getSelectedBank())
    {
      Glib::ustring name = request->get ("name");
      Glib::ustring info = request->get ("info");

      UNDO::Scope::tTransactionScopePtr scope = m_presetManager.getUndoScope().startTransaction ("Insert preset");
      UNDO::Scope::tTransactionPtr transaction = scope->getTransaction();

      int selPreset = bank->getPresetPosition (bank->getSelectedPreset());
      int desiredPresetPos = selPreset + 1;
      bank->undoableInsertPreset (transaction, desiredPresetPos);
      bank->undoableOverwritePreset (transaction, desiredPresetPos, m_presetManager.getEditBuffer());
      auto p = bank->getPreset (desiredPresetPos);
      bank->undoableSelectPreset (transaction, p->getUuid());
      p->undoableSetName (transaction, name);
      p->undoableSetAttribute(transaction, "Comment", info);
      m_presetManager.undoableSelectBank (transaction, bank->getUuid());
    }
  });

  addAction("select-preset",
      [&] (shared_ptr<NetworkRequest> request) mutable
      {
        Glib::ustring presetUUID = request->get ("uuid");

        if (tBankPtr bank = m_presetManager.findBankWithPreset (presetUUID))
        {
          if(auto preset = bank->getPreset(presetUUID))
          {
            UNDO::Scope::tTransactionScopePtr scope;

            bool autoLoad = Application::get().getSettings ()->getSetting<AutoLoadSelectedPreset> ()->get ();

            if(autoLoad)
            scope = m_presetManager.getUndoScope().startTransaction (preset->getUndoTransactionTitle("Load"));
            else
            scope = m_presetManager.getUndoScope().startContinuousTransaction (&presetManager, chrono::hours(1), preset->getUndoTransactionTitle("Select"));

            UNDO::Scope::tTransactionPtr transaction = scope->getTransaction();
            m_presetManager.undoableSelectBank (transaction, bank->getUuid());
            bank->undoableSelectPreset (transaction, presetUUID);
          }
        }
      });

  addAction("delete-preset",
      [&] (shared_ptr<NetworkRequest> request) mutable
      {
        Glib::ustring presetUUID = request->get ("uuid");

        if (tBankPtr srcBank = m_presetManager.findBankWithPreset (presetUUID))
        {
          if(auto preset = srcBank->getPreset(presetUUID))
          {
            UNDO::Scope::tTransactionScopePtr scope = m_presetManager.getUndoScope().startTransaction (preset->getUndoTransactionTitle("Delete"));
            UNDO::Scope::tTransactionPtr transaction = scope->getTransaction();
            srcBank->undoableDeletePreset (transaction, presetUUID);

            if(srcBank->getNumPresets() == 0)
            {
              m_presetManager.undoableDeleteBank(transaction, srcBank);
            }

            m_presetManager.getEditBuffer()->undoableUpdateLoadedPresetInfo (scope->getTransaction());
          }
        }
      });

  addAction("delete-presets", [&] (shared_ptr<NetworkRequest> request) mutable
  {
    UNDO::Scope::tTransactionScopePtr scope = m_presetManager.getUndoScope().startTransaction ("Delete Presets");
    UNDO::Scope::tTransactionPtr transaction = scope->getTransaction();

    vector<string> strs;
    Glib::ustring csv = request->get ("presets");
    boost::split(strs, csv, boost::is_any_of(","));

    for(auto presetUUID : strs)
    {
      if (tBankPtr srcBank = m_presetManager.findBankWithPreset (presetUUID))
      {
        if(auto preset = srcBank->getPreset(presetUUID))
        {
          srcBank->undoableDeletePreset (transaction, presetUUID);

          if(srcBank->getNumPresets() == 0)
          {
            m_presetManager.undoableDeleteBank(transaction, srcBank);
          }

          m_presetManager.getEditBuffer()->undoableUpdateLoadedPresetInfo (scope->getTransaction());
        }
      }
    }
  });

  addAction("load-preset", [&] (shared_ptr<NetworkRequest> request) mutable
  {
    Glib::ustring uuid = request->get ("uuid");

    if (tBankPtr bank = m_presetManager.findBankWithPreset (uuid))
    {
      if (PresetBank::tPresetPtr preset = bank->getPreset (uuid))
      {
        UNDO::Scope::tTransactionScopePtr scope = m_presetManager.getUndoScope().startTransaction (preset->getUndoTransactionTitle("Load"));
        UNDO::Scope::tTransactionPtr transaction = scope->getTransaction();
        m_presetManager.getEditBuffer()->undoableLoad (transaction, preset);
        preset->undoableSelect (transaction);
      }
    }
  });

  addAction("set-position",
      [&] (shared_ptr<NetworkRequest> request) mutable
      {
        Glib::ustring uuid = request->get ("uuid");
        Glib::ustring x = request->get ("x");
        Glib::ustring y = request->get ("y");

        if (tBankPtr bank = m_presetManager.findBank (uuid))
        {
          if(bank->getX() != x || bank->getY() != y)
          {
            UNDO::Scope::tTransactionScopePtr scope = presetManager.getUndoScope().startTransaction ("Move preset bank '%0'", bank->getName(true));
            UNDO::Scope::tTransactionPtr transaction = scope->getTransaction();

            bank->undoableSetPosition (transaction, x, y);
            //  m_presetManager.undoableSelectBank (transaction, bank->getUuid(), false);
          }
        }
      });

  addAction("create-new-bank-from-preset", [&] (shared_ptr<NetworkRequest> request) mutable
  {
    Glib::ustring uuid = request->get ("preset");
    Glib::ustring x = request->get ("x");
    Glib::ustring y = request->get ("y");

    if (tBankPtr bank = m_presetManager.findBankWithPreset (uuid))
    {
      if (PresetBank::tPresetPtr p = bank->getPreset (uuid))
      {
        UNDO::Scope::tTransactionScopePtr scope = m_presetManager.getUndoScope().startTransaction ("Create new bank");
        PresetManager::tBankPtr newBank = presetManager.addBank (scope->getTransaction(), x, y);
        auto transaction = scope->getTransaction();
        newBank->undoableInsertPreset (transaction, 0);
        newBank->undoableOverwritePreset (transaction, 0, p);
        newBank->undoableSetName(transaction, "New Bank");
        newBank->getPreset (0)->undoableSelect (transaction);
        m_presetManager.undoableSelectBank (transaction, newBank->getUuid());
      }
    }
  });

  addAction("create-new-bank-from-presets", [&] (shared_ptr<NetworkRequest> request) mutable
  {
    Glib::ustring csv = request->get ("presets");
    Glib::ustring x = request->get ("x");
    Glib::ustring y = request->get ("y");

    UNDO::Scope::tTransactionScopePtr scope = m_presetManager.getUndoScope().startTransaction ("Create new bank");
    auto transaction = scope->getTransaction();

    PresetManager::tBankPtr newBank = presetManager.addBank (transaction, x, y);
    newBank->undoableSetName(transaction, "New Bank");

    vector<string> strs;
    boost::split(strs,csv,boost::is_any_of(","));

    for(auto uuid : strs)
    {
      if(auto src = presetManager.findPreset(uuid))
      {
        auto pos = newBank->getNumPresets();
        newBank->undoableInsertPreset (transaction, pos);
        newBank->undoableOverwritePreset (transaction, pos, src);
      }
    }

    if(newBank->getNumPresets() > 0)
    {
      newBank->getPreset (0)->undoableSelect (transaction);
    }

    m_presetManager.undoableSelectBank (transaction, newBank->getUuid());
  });

  addAction("next-preset", [&] (shared_ptr<NetworkRequest> request) mutable
  {
    loadPresetAtRelativePosition (1);
  });

  addAction("prev-preset", [&] (shared_ptr<NetworkRequest> request) mutable
  {
    loadPresetAtRelativePosition (-1);
  });

  addAction("overwrite-selected-preset",
      [&] (shared_ptr<NetworkRequest> request) mutable
      {
        if (tBankPtr tgtBank = m_presetManager.getSelectedBank ())
        {
          Glib::ustring selPresetUUID = tgtBank->getSelectedPreset();

          if(auto tgtPreset = tgtBank->getPreset(selPresetUUID))
          {
            size_t anchorPos = tgtBank->getPresetPosition (selPresetUUID);
            UNDO::Scope::tTransactionScopePtr scope = m_presetManager.getUndoScope().startTransaction ("Overwrite preset '%0'", tgtPreset->getName());
            auto transaction = scope->getTransaction();
            tgtBank->undoableOverwritePreset (transaction, anchorPos, m_presetManager.getEditBuffer());
            tgtBank->getPreset (anchorPos)->undoableSelect (transaction);
            tgtBank->undoableSelect (transaction);
          }
        }
      });

  addAction("drop-bank-on-bank", [&] (shared_ptr<NetworkRequest> request) mutable
  {
    Glib::ustring receiverUuid = request->get ("receiver");
    Glib::ustring bankUuid = request->get ("bank");

    if (tBankPtr receiver = m_presetManager.findBank (receiverUuid))
    if (tBankPtr bank = m_presetManager.findBank (bankUuid))
    if (bank != receiver)
    insertBank (bank, receiver, receiver->getNumPresets());
  });

  addAction("drop-presets-on-bank", [&] (shared_ptr<NetworkRequest> request) mutable
  {
    Glib::ustring bankUUID = request->get ("bank");
    Glib::ustring csv = request->get ("presets");

    if (tBankPtr bank = m_presetManager.findBank (bankUUID))
    {
      UNDO::Scope::tTransactionScopePtr scope = m_presetManager.getUndoScope().startTransaction ("Drop Presets on Bank");
      auto transaction = scope->getTransaction();

      vector<string> strs;
      boost::split(strs,csv,boost::is_any_of(","));

      for(auto uuid : strs)
      {
        if(auto src = presetManager.findPreset(uuid))
        {
          if(bank.get() == src->getParent())
          {
            bank->undoableDeletePreset(transaction, uuid);
          }

          auto pos = bank->getNumPresets();
          bank->undoableInsertPreset (transaction, pos);
          bank->undoableOverwritePreset (transaction, pos, src);
        }
      }
    }
  });

  addAction("insert-bank-above", [&] (shared_ptr<NetworkRequest> request) mutable
  {
    insertBank (request, 0);
  });

  addAction("insert-bank-below", [&] (shared_ptr<NetworkRequest> request) mutable
  {
    insertBank (request, 1);
  });

  addAction("overwrite-preset-and-rename",
      [&] (shared_ptr<NetworkRequest> request)
      {
        Glib::ustring newName = request->get ("name");
        Glib::ustring info = request->get ("info");

        if (tBankPtr tgtBank = m_presetManager.getSelectedBank ())
        {
          Glib::ustring selPresetUUID = tgtBank->getSelectedPreset();

          if(auto tgtPreset = tgtBank->getPreset(selPresetUUID))
          {
            size_t anchorPos = tgtBank->getPresetPosition (selPresetUUID);
            UNDO::Scope::tTransactionScopePtr scope = m_presetManager.getUndoScope().startTransaction ("Overwrite preset '%0'", tgtPreset->getName());
            auto transaction = scope->getTransaction();
            tgtBank->undoableOverwritePreset (transaction, anchorPos, m_presetManager.getEditBuffer());

            auto p = tgtBank->getPreset (anchorPos);
            p->undoableSelect (transaction);
            p->undoableSetName (transaction, newName);
            p->undoableSetAttribute(transaction, "Comment", info);

            tgtBank->undoableSelect (transaction);
          }
        }
      });

  addAction("overwrite-preset-with-bank", [&] (shared_ptr<NetworkRequest> request) mutable
  {
    Glib::ustring anchorUuid = request->get ("anchor");
    Glib::ustring bankUuid = request->get ("bank");

    if (tBankPtr targetBank = m_presetManager.findBankWithPreset (anchorUuid))
    if (tBankPtr bank = m_presetManager.findBank (bankUuid))
    if (bank != targetBank)
    {
      auto &undo = m_presetManager.getUndoScope ();
      auto scope = undo.startTransaction ("Drop bank '%0' into bank '%1'", bank->getName (true), targetBank->getName (true));

      size_t insertPos = targetBank->getPresetPosition (anchorUuid) + 1;
      size_t numPresets = bank->getNumPresets ();

      for (size_t i = 0; i < numPresets; i++)
      {
        targetBank->undoableInsertPreset (scope->getTransaction (), insertPos);
        targetBank->getPreset (insertPos)->copyFrom (scope->getTransaction (), bank->getPreset (i).get(), true);
        insertPos++;
      }

      targetBank->undoableDeletePreset (scope->getTransaction (), anchorUuid);
    }
  });

  addAction("import-bank", [&] (shared_ptr<NetworkRequest> request) mutable
  {
    ustring xml = request->get ("xml");
    Glib::ustring x = request->get ("x");
    Glib::ustring y = request->get ("y");
    Glib::ustring fileName = request->get ("fileName");
    Glib::ustring lastModified = request->get ("lastModified");

    MemoryInStream stream (xml, false);
    auto newBank = importBank(stream, x, y, fileName, lastModified);
  });

  addAction("duplicate-bank",
      [&] (shared_ptr<NetworkRequest> request) mutable
      {
        Glib::ustring uuid = request->get ("uuid");

        if (tBankPtr bank = m_presetManager.findBank (uuid))
        {
          UNDO::Scope::tTransactionScopePtr scope = presetManager.getUndoScope().startTransaction ("Duplicate preset bank '%0'", bank->getName(true));
          UNDO::Scope::tTransactionPtr transaction = scope->getTransaction();
          tBankPtr newBank = presetManager.addBank(transaction, true);
          newBank->copyFrom(transaction, bank, true);
          newBank->undoableMovePosition (transaction, 20, 20);
        }
      });

  addAction("sort-bank",
      [&] (shared_ptr<NetworkRequest> request) mutable
      {
        Glib::ustring uuid = request->get ("uuid");
        Glib::ustring asc = request->get ("asc");

        if (tBankPtr bank = m_presetManager.findBank (uuid))
        {
          UNDO::Scope::tTransactionScopePtr scope = presetManager.getUndoScope().startTransaction ("Sort preset bank '%0'", bank->getName(true));
          UNDO::Scope::tTransactionPtr transaction = scope->getTransaction();
          bank->undoableSort(transaction, stoi(asc) > 0);
        }
      });

  addAction("set-preset-attribute", [&] (shared_ptr<NetworkRequest> request) mutable
  {
    Glib::ustring presetUUID = request->get ("uuid");
    Glib::ustring key = request->get ("key");
    Glib::ustring value = request->get ("value");

    if(auto preset = m_presetManager.findPreset(presetUUID))
    {
      UNDO::Scope::tTransactionScopePtr scope = presetManager.getUndoScope().startTransaction ("Set preset attribute");
      UNDO::Scope::tTransactionPtr transaction = scope->getTransaction();
      preset->undoableSetAttribute(transaction, key, value);
    }
  });

  addAction("set-bank-attribute", [&] (shared_ptr<NetworkRequest> request) mutable
  {
    Glib::ustring bankUUID = request->get ("uuid");
    Glib::ustring key = request->get ("key");
    Glib::ustring value = request->get ("value");

    if(auto bank = m_presetManager.findBank(bankUUID))
    {
      UNDO::Scope::tTransactionScopePtr scope = presetManager.getUndoScope().startTransaction ("Set bank attribute");
      UNDO::Scope::tTransactionPtr transaction = scope->getTransaction();
      bank->undoableSetAttribute(transaction, key, value);
    }
  });

  addAction("move", [&] (shared_ptr<NetworkRequest> request) mutable
  {
    Glib::ustring bankUUID = request->get ("bank");
    Glib::ustring value = request->get ("by");

    if (tBankPtr bank = m_presetManager.getSelectedBank())
    {
      UNDO::Scope::tTransactionScopePtr scope = presetManager.getUndoScope().startTransaction ("Move Bank '%0'", bank->getName(true));
      UNDO::Scope::tTransactionPtr transaction = scope->getTransaction();
      m_presetManager.undoableMoveBankBy(transaction, bankUUID, stoi(value));
    }
  });

  addAction("dock-bank", [&] (shared_ptr<NetworkRequest> request) mutable
  {
    const auto uuid = request->get("uuid");
    const auto masterUuid = request->get("master-uuid");
    const auto dockingDirection = request->get("direction");

    DebugLevel::warning("docking-bank: ", uuid, " to ", masterUuid, " side: ", dockingDirection);

    if(tBankPtr bank = m_presetManager.findBank(uuid))
    {
      auto newMasterName = m_presetManager.findBank(masterUuid)->getName(true);
      auto scope = presetManager.getUndoScope().startTransaction("Attached Bank '%0' to '%1'", bank->getName(true), newMasterName);
      auto transaction = scope->getTransaction();
      auto direction = dockingDirection == "top" ? PresetBank::AttachmentDirection::top : PresetBank::AttachmentDirection::left;
      bank->undoableAttachBank(transaction, masterUuid, direction);
      m_presetManager.sanitizeBankClusterRelations(transaction);
    }
  });

  addAction("dock-banks", [&] (shared_ptr<NetworkRequest> request) mutable
  {
    const auto droppedOntoBankUuid = request->get("droppedOntoBank");
    const auto draggedBankUuid = request->get("draggedBank");
    const auto droppedAt = request->get("droppedAt");
    const auto x = request->get("x");
    const auto y = request->get("y");

    if(tBankPtr droppedOntoBank = m_presetManager.findBank(droppedOntoBankUuid))
    {
      if(tBankPtr draggedBank = m_presetManager.findBank(draggedBankUuid))
      {
        auto scope = presetManager.getUndoScope().startTransaction("Dock Banks '%0' and '%1'", droppedOntoBank->getName(true), draggedBank->getName(true));
        auto transaction = scope->getTransaction();

        if(droppedAt == "North")
        {
          droppedOntoBank->undoableAttachBank(transaction, draggedBank->getUuid(), PresetBank::AttachmentDirection::top);
          draggedBank->getClusterMaster()->undoableSetPosition(transaction, x, y);
        }
        else if(droppedAt == "West")
        {
          droppedOntoBank->undoableAttachBank(transaction, draggedBank->getUuid(), PresetBank::AttachmentDirection::left);
          draggedBank->getClusterMaster()->undoableSetPosition(transaction, x, y);
        }
        else if(droppedAt == "South")
        {
          draggedBank->undoableAttachBank(transaction, droppedOntoBank->getUuid(), PresetBank::AttachmentDirection::top);
        }
        else if(droppedAt == "East")
        {
          draggedBank->undoableAttachBank(transaction, droppedOntoBank->getUuid(), PresetBank::AttachmentDirection::left);
        }

        m_presetManager.sanitizeBankClusterRelations(transaction);
      }
    }
  });

  addAction("undock-bank", [&] (shared_ptr<NetworkRequest> request) mutable
  {
    const auto uuid = request->get("uuid");
    const auto x = request->get("x");
    const auto y = request->get("y");

    if(auto bank = m_presetManager.findBank(uuid))
    {
      if(auto attached = m_presetManager.findBank(bank->getAttached().uuid))
      {
        auto parentBankName = attached->getName(true);
        auto scope = presetManager.getUndoScope().startTransaction("Detached Bank '%0' from '%1'", bank->getName(true), parentBankName);
        auto transaction = scope->getTransaction();
        bank->undoableDetachBank(transaction);
        bank->undoableSetPosition(transaction, x, y);
      }
    }
  });

  addAction("move-all-banks", [&](shared_ptr<NetworkRequest> request) mutable
  {
    auto x = atof(request->get("x").c_str());
    auto y = atof(request->get("y").c_str());

    UNDO::Scope::tTransactionScopePtr scope = m_presetManager.getUndoScope().startTransaction("Move all Banks");
    UNDO::Scope::tTransactionPtr transaction = scope->getTransaction();

    for(auto bank: m_presetManager.getBanks())
    {
      bank->undoableMovePosition(transaction, x, y);
    }
  });
}

BankActions::~BankActions()
{
}

void BankActions::dropPresets(UNDO::TransactionCreationScope::tTransactionPtr transaction, const Glib::ustring &anchorUUID, int offset,
                              const Glib::ustring &csv)
{
  vector<string> strs;
  boost::split(strs, csv, boost::is_any_of(","));

  if(auto anchor = m_presetManager.findPreset(anchorUUID))
  {
    auto bank = anchor->getBank();
    auto pos = bank->getPresetPosition(anchorUUID) + offset;

    for(auto presetUUID : strs)
    {
      if(auto src = m_presetManager.findPreset(presetUUID))
      {
        if(src->getBank() == anchor->getBank())
        {
          auto oldPos = bank->getPresetPosition(presetUUID);
          auto preset = bank->undoableExpropriatePreset(transaction, presetUUID);

          if(oldPos < pos)
            pos--;

          bank->undoableAdoptPreset(transaction, pos, preset);
          pos++;
        }
        else
        {
          bank->undoableInsertPreset(transaction, pos);
          bank->undoableOverwritePreset(transaction, pos, src);
          pos++;
        }
      }
    }
  }
}

void BankActions::insertBank(shared_ptr<NetworkRequest> request, size_t offset)
{
  Glib::ustring anchorUuid = request->get("anchor");
  Glib::ustring bankUuid = request->get("bank");

  if(tBankPtr targetBank = m_presetManager.findBankWithPreset(anchorUuid))
    if(tPresetPtr preset = targetBank->getPreset(anchorUuid))
      if(tBankPtr bank = m_presetManager.findBank(bankUuid))
        if(bank != targetBank)
          insertBank(bank, targetBank, targetBank->getPresetPosition(anchorUuid) + offset);
}

void BankActions::insertBank(tBankPtr bank, tBankPtr targetBank, size_t insertPos)
{
  UNDO::Scope::tTransactionScopePtr scope = m_presetManager.getUndoScope().startTransaction("Drop bank '%0' into bank '%1'",
      bank->getName(true), targetBank->getName(true));
  size_t numPresets = bank->getNumPresets();

  for(size_t i = 0; i < numPresets; i++)
  {
    targetBank->undoableInsertPreset(scope->getTransaction(), insertPos);
    targetBank->getPreset(insertPos)->copyFrom(scope->getTransaction(), bank->getPreset(i).get(), true);
    insertPos++;
  }
}

bool BankActions::loadPresetAtRelativePosition(int offset)
{
  Glib::ustring currentPreset = m_presetManager.getEditBuffer()->getUUIDOfLastLoadedPreset();

  if(tBankPtr bank = m_presetManager.findBankWithPreset(currentPreset))
  {
    size_t pos = bank->getPresetPosition(currentPreset);

    if(offset > 0 || pos >= (size_t) (-offset))
    {
      size_t newPos = pos + offset;

      if(newPos < bank->getNumPresets())
      {
        tPresetPtr newPreset = bank->getPreset(newPos);
        UNDO::Scope::tTransactionScopePtr scope = m_presetManager.getUndoScope().startTransaction(
            newPreset->getUndoTransactionTitle("Load"));
        UNDO::Scope::tTransactionPtr transaction = scope->getTransaction();
        m_presetManager.getEditBuffer()->undoableLoad(transaction, newPreset);
        newPreset->undoableSelect(scope->getTransaction());
        return true;
      }
    }
  }

  return false;
}

bool BankActions::handleRequest(const Glib::ustring &path, shared_ptr<NetworkRequest> request)
{
  if(super::handleRequest(path, request))
    return true;

  if(path.find("/presets/banks/download-bank/") == 0)
  {
    PerformanceTimer timer(__PRETTY_FUNCTION__);

    if(auto httpRequest = dynamic_pointer_cast<HTTPRequest>(request))
    {
      Glib::ustring uuid = request->get("uuid");

      if(tBankPtr bank = m_presetManager.findBank(uuid))
      {
        auto stream = request->createStream("text/xml", false);
        httpRequest->setHeader("Content-Disposition", "attachment; filename=\"" + bank->getName(true) + ".xml\"");
        XmlWriter writer(stream);
        PresetBankSerializer serializer(bank);
        serializer.write(writer, VersionAttribute::get());

        bank->setAttribute("Name of Export File", "via WebUI");
        bank->setAttribute("Date of Export File", DateTimeInfo::getIsoStringOfNow());

      }

      return true;
    }
  }

  return false;
}

PresetManager::tBankPtr BankActions::importBank(InStream& stream, const Glib::ustring& x, const Glib::ustring& y,
                                                const Glib::ustring& fileName, const Glib::ustring& lastModified)
{

  UNDO::Scope::tTransactionScopePtr scope = m_presetManager.getUndoScope().startTransaction("Import new Bank");
  auto transaction = scope->getTransaction();
  PresetManager::tBankPtr newBank = m_presetManager.addBank(transaction, x, y);

  XmlReader reader(stream, transaction);
  reader.read<PresetBankSerializer>(newBank, true);

  if(x.empty() || y.empty())
  {
    newBank->undoableAssignDefaultPosition(transaction);
  }
  else
  {
    newBank->undoableSetPosition(transaction, x, y);
  }

  newBank->undoableEnsurePresetSelection(transaction);
  newBank->undoableSetAttribute(transaction, "Name of Import File", fileName);
  auto lastModifiedSeconds = stoull(lastModified) / 1000;
  newBank->undoableSetAttribute(transaction, "Date of Import File", DateTimeInfo::getIsoStringOfNow());
  newBank->undoableSetAttribute(transaction, "Name of Export File", "");
  newBank->undoableSetAttribute(transaction, "Date of Export File", "");
  newBank->undoableSelect(transaction);
  m_presetManager.undoableSelectBank(transaction, newBank->getUuid());
  return newBank;
}
