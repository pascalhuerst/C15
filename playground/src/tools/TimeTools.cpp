#include "TimeTools.h"

#include <ctime>
#include <iomanip>
#include <memory>
#include <sstream>

#include <device-settings/DebugLevel.h>

time_t TimeTools::getRealTimestamp()
{
  return time(nullptr);
}

time_t TimeTools::getAdjustedTimestamp()
{
  auto dateTimeSetting
      = static_pointer_cast<DateTimeAdjustment>(Application::get().getSettings()->getSetting("DateTimeAdjustment"));
  return getRealTimestamp() + dateTimeSetting->get();
}

Glib::ustring TimeTools::getRealIso()
{
  return getIsoTime(getRealTimestamp());
}

Glib::ustring TimeTools::getAdjustedIso()
{
  return getIsoTime(getAdjustedTimestamp());
}

Glib::ustring TimeTools::getIsoTime(const time_t stamp)
{
  char buf[128];
  strftime(buf, sizeof buf, "%FT%TZ", gmtime(&stamp));
  return buf;
}

Glib::ustring TimeTools::getDisplayStringFromIso(const Glib::ustring &iso)
{
  return formatTime(iso, "%F %R");
}

Glib::ustring TimeTools::getDisplayStringFromStamp(time_t stamp)
{
  return formatTime(stamp, "%F %R");
}

Glib::ustring TimeTools::formatTime(time_t secondsSinceUnixEpoch, const Glib::ustring &format)
{
  auto tm = gmtime(&secondsSinceUnixEpoch);
  return formatTime(tm, format);
}

Glib::ustring TimeTools::formatTime(const Glib::ustring &iso, const Glib::ustring &format)
{
  struct tm t;

  if(!strptime(iso.raw().c_str(), "%Y-%m-%dT%H:%M:%S%z", &t))
  {
    DebugLevel::warning("could not parse time value", iso);
    return "---";
  }

  return formatTime(&t, format);
}

Glib::ustring TimeTools::formatTime(const tm *tm, const Glib::ustring &format)
{
  std::stringstream ss;
  ss << std::put_time(tm, format.c_str());
  return ss.str();
}

Glib::ustring TimeTools::getPerformanceTimeStamp()
{
  struct timeval te;
  gettimeofday(&te, NULL);
  uint64_t milliseconds = te.tv_sec * 1000LL + te.tv_usec / 1000;
  auto seconds = milliseconds / 1000;
  milliseconds -= seconds * 1000;
  auto minutes = seconds / 60;
  seconds -= minutes * 60;
  auto hours = minutes / 60;
  minutes -= hours * 60;
  auto days = hours / 24;
  hours -= days * 24;

  char txt[256];
  sprintf(txt, "%zu days, %zu:%02zu:%02zu.%03zu", days, hours, minutes, seconds, milliseconds);
  return txt;
}
