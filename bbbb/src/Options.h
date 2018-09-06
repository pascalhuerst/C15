#pragma once

#include "bbbb.h"
#include <giomm/file.h>

class Options
{
 public:
  Options(int &argc, char **&argv);
  virtual ~Options();

  inline bool isTurnAroundStopWatchEnabled() const
  {
    return m_bTurnAroundStopWatch;
  }

 private:
  bool m_bTurnAroundStopWatch = false;
};
