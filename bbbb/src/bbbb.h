#pragma once

#include <glib.h>
#include <glibmm.h>
#include <giomm.h>
#include <sigc++/sigc++.h>
#include <sigc++/trackable.h>
#include <memory>
#include <atomic>
#include <stdio.h>
#include <iostream>
#include <sys/time.h>

#define TRACE_LOC_AND_ARGS(loc, arg) std::cout << loc << ": " << arg << std::endl
#define TRACE(arg) TRACE_LOC_AND_ARGS(G_STRLOC, arg)
#define TRACE_CALL() TRACE_LOC_AND_ARGS(G_STRLOC, "called")

enum class Domain : uint8_t
{
  Lpc = 1,
  Buttons = 2,
  Oled = 3,
  Rotary = 4,
  PanelLed = 5,
  RibbonLed = 6
};

inline Glib::ustring getPerformanceTimeStamp()
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
