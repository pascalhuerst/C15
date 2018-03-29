#pragma once

#include <playground.h>

namespace DescriptiveLayouts
{
  enum class EventSinks
  {
    incParam, decParam, incParamFine, decParamFine,
  };

  class EventSinkBroker
  {
    public:
      static EventSinkBroker& get();

    private:
      EventSinkBroker();

      struct EventSink
      {
          std::function<void()> doAction;
      };

      std::unordered_map<EventSinks, EventSink> m_map;
  };
}
