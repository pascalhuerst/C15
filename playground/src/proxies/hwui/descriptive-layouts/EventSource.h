#pragma once

#include <playground.h>
#include <any>

namespace DescriptiveLayouts
{
  enum class EventSources
  {
    parameterName, parameterValue, parameterDisplayString, paramterGroupName
  };

  class EventSourceBroker
  {
    public:
      static EventSourceBroker& get();

    private:
      EventSourceBroker();

      struct EventSource
      {
          sigc::connection m_inputConnection;
          Signal<void, std::any> m_outputSignal;
      };

      std::unordered_map<EventSources, EventSource> m_map;
  };
}
