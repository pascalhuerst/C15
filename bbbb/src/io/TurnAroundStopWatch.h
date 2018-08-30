#pragma once

#include <string>
#include <chrono>

class TurnAroundStopWatch
{
  public:
    static TurnAroundStopWatch& get(uint32_t id);
    static void stopAndRemove(uint32_t id);

    TurnAroundStopWatch(uint32_t id);
    virtual ~TurnAroundStopWatch();

    void start();
    void stop();

  private:
    uint32_t m_id;
    std::chrono::high_resolution_clock::time_point m_start;
};

