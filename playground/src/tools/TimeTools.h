#pragma once

#include <glibmm/ustring.h>
#include <Application.h>
#include <device-settings/Settings.h>
#include <device-settings/DateTimeAdjustment.h>
#include <chrono>

class TimeTools
{
  public:
    static Glib::ustring getAdjustedIso();
    static time_t getAdjustedTimestamp();

    static Glib::ustring getDisplayStringFromIso(const Glib::ustring & iso);
    static Glib::ustring getDisplayStringFromStamp(time_t stamp);
    static Glib::ustring getIsoTime(const time_t stamp);

    static Glib::ustring getPerformanceTimeStamp();

   private:
    static Glib::ustring formatTime(time_t secondsSinceUnixEpoch, const Glib::ustring &format);
    static Glib::ustring formatTime(const Glib::ustring &iso, const Glib::ustring &format);
    static Glib::ustring formatTime(const tm *tm, const Glib::ustring &format);

    static time_t getRealTimestamp();
    static Glib::ustring getRealIso();
};
