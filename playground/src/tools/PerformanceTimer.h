#pragma once

#include <chrono>
#include "device-settings/DebugLevel.h"

using namespace std::chrono;

class PerformanceTimer
{
    using Clock = system_clock;

  public:
    PerformanceTimer (const char *name, int indent = 0) :
      m_name (name),
      m_indent (indent),
      m_start (Clock::now())
    {
      s_indent += m_indent;
    }

    ~PerformanceTimer ()
    {
      s_indent -= m_indent;
      auto diff = Clock::now () - m_start;
      auto msec = duration_cast<milliseconds> (diff);
      
      if(m_indent == 0)
      {
        DebugLevel::warning(m_name, "took", msec.count(), "ms");
      }
      else 
      {
        Glib::ustring indentation(s_indent, ' ');
        DebugLevel::warning(indentation, m_name, "took", msec.count(), "ms");
      }
    }

  private:
    const char *m_name;
    Clock::time_point m_start;
    static int s_indent;
    int m_indent = 0;
};
