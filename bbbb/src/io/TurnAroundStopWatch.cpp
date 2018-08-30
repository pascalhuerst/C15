#include "TurnAroundStopWatch.h"
#include "bbbb.h"
#include <map>
#include <memory>
#include <iostream>
#include <algorithm>

using namespace std::chrono;

static std::map<uint32_t, high_resolution_clock::time_point> watches;
static std::array<high_resolution_clock::duration, 128> ringBuffer = {};
static size_t ringBufferIdx = 0;

static std::array<high_resolution_clock::duration, 32> maxRingBuffer = {};
static size_t maxRingBufferIdx = 0;

void TurnAroundStopWatch::stop(ClockID* ids, uint32_t numClocks)
{
  if(numClocks)
  {
    auto stop = high_resolution_clock::now();

    for(uint32_t packetID = 0; packetID < numClocks; packetID++)
    {
      auto id = ids[packetID];
      auto it = watches.find(id);

      if(it != watches.end())
      {
        auto diff = stop - it->second;

        std::cerr << "bbbb rendered encoder event " << id << " at " << getPerformanceTimeStamp() << std::endl;

        if(diff > milliseconds(20))
          G_BREAKPOINT();

        ringBuffer[ringBufferIdx++ & (ringBuffer.size() - 1)] = diff;
        watches.erase(it);
      }
    }

    auto maxElement = std::max_element(ringBuffer.begin(), ringBuffer.end());

    if((ringBufferIdx % ringBuffer.size()) == 0)
    {
      maxRingBuffer[maxRingBufferIdx++ & (maxRingBuffer.size() - 1)] = *maxElement;
    }

#if 0
    std::cerr << "\r";

    for(auto p = 1; p < maxRingBuffer.size(); p++)
    {
      auto id = (maxRingBufferIdx - p) & (maxRingBuffer.size() - 1);
      std::cerr << duration_cast<milliseconds>(maxRingBuffer[id]).count() << ", ";
    }
#endif
  }
}

TurnAroundStopWatch::ClockID TurnAroundStopWatch::start()
{
  static ClockID sID = 0;
  watches[++sID] = high_resolution_clock::now();
  return sID;
}
