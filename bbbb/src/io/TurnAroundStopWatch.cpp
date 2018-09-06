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

static std::array<high_resolution_clock::duration, 16> maxRingBuffer = {};
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
        std::cout << "TurnAroundStopWatch received packet " << id << " at "
                  << duration_cast<milliseconds>(stop.time_since_epoch()).count() << std::endl;

        auto diff = stop - it->second;
        ringBuffer[ringBufferIdx++ & (ringBuffer.size() - 1)] = diff;
        watches.erase(it);
      }
      else
      {
        std::cerr << "did not find clock with id " << id << std::endl;
      }
    }

    auto maxElement = std::max_element(ringBuffer.begin(), ringBuffer.end());

    if((ringBufferIdx % ringBuffer.size()) == 0)
    {
      maxRingBuffer[maxRingBufferIdx++ & (maxRingBuffer.size() - 1)] = *maxElement;
    }

    std::cerr << "\r";

    for(auto p = 1; p < maxRingBuffer.size(); p++)
    {
      auto id = (maxRingBufferIdx - p) & (maxRingBuffer.size() - 1);
      std::cerr << duration_cast<milliseconds>(maxRingBuffer[id]).count();

      if(p < (maxRingBuffer.size() - 1))
        std::cerr << ", ";
    }
  }
}

TurnAroundStopWatch::ClockID TurnAroundStopWatch::start()
{
  static ClockID sID = 0;

  sID++;
  auto start = high_resolution_clock::now();
  watches[sID] = start;
  std::cout << "Send packet " << sID << " at " << duration_cast<milliseconds>(start.time_since_epoch()).count()
            << std::endl;
  return sID;
}
