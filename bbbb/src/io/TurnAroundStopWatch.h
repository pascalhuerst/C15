#pragma once

#include <string>
#include <chrono>

class TurnAroundStopWatch
{
 public:
  using ClockID = uint32_t;
  static ClockID start();
  static void stop(ClockID *ids, uint32_t numClocks);
};
