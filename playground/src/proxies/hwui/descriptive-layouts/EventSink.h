#pragma once

#include <playground.h>
#include "TemplateEnums.h"

namespace DescriptiveLayouts
{
  enum class EventSinks
  {
    IncParam, DecParam, IncParamFine, DecParamFine,
  };

  struct EventSinkMapping
  {
    public:
      EventSinkMapping(int button, EventSinks sink, ButtonEvents event = ButtonEvents::Down, ButtonModifiers modifiers =
          ButtonModifiers::None);

    private:
      int button;
      ButtonEvents event;
      ButtonModifiers modifiers;
      EventSinks sink;
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
