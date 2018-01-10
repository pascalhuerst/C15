#pragma once

#include "DeviceInformationItem.h"

class DateTimeInfo : public DeviceInformationItem
{
  public:
    DateTimeInfo (DeviceInformation *parent);

    virtual void writeDocument (Writer &writer, tUpdateID knownRevision) const override;
    Glib::ustring get () const override;
    Glib::ustring getDisplayString () const override;

    static Glib::ustring formatTime (uint64_t secondsSinceUnixEpoch, const Glib::ustring &format);
    static Glib::ustring formatTime (const Glib::ustring &iso, const Glib::ustring &format);

  private:
    bool onTimeChanged ();
    Glib::ustring formatTime (const std::string &format) const;
};
