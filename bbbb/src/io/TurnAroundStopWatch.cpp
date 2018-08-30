#include "TurnAroundStopWatch.h"
#include <map>
#include <memory>
#include <iostream>

using namespace std::chrono;
static std::map<uint32_t, std::unique_ptr<TurnAroundStopWatch>> watches;

TurnAroundStopWatch::TurnAroundStopWatch(uint32_t id) :
    m_id(id)
{
  m_start = high_resolution_clock::now();
}

TurnAroundStopWatch::~TurnAroundStopWatch()
{
}

void TurnAroundStopWatch::stopAndRemove(uint32_t id)
{
  auto it = watches.find(id);
  if(it != watches.end())
  {
    it->second->stop();
    watches.erase(it);
  }
}

void TurnAroundStopWatch::start()
{
  m_start = high_resolution_clock::now();
}

void TurnAroundStopWatch::stop()
{
  using namespace std::chrono;
  auto stop = high_resolution_clock::now();
  auto diff = stop - m_start;
  auto ms = duration_cast<milliseconds>(diff).count();
  std::cerr << "TurnAroundStopWatch " << m_id << " took " << ms << " ms" << std::endl;
}

TurnAroundStopWatch& TurnAroundStopWatch::get(uint32_t id)
{
  auto it = watches.find(id);
  if(it == watches.end())
  {
    watches[id] = std::make_unique<TurnAroundStopWatch>(id);
    return get(id);
  }
  else
  {
    return *(it->second.get());
  }
}

