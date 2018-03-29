#pragma once


#include "EventSource.h"

struct EventSink {
  std::function<void()> doAction;
};


enum class EventSinkEnum {
    incParam,
    decParam,
    incFineParam,
    decFineParam,
};

class EventSinkBroker {
private:
    EventSinkBroker() {
      //TODO fill me pls m_map
    };
public:
    static EventSinkBroker& get() {
      static EventSinkBroker s;
      return s;
    }

    std::unordered_map<EventSinkEnum, EventSink> m_map;
};

struct EventSinkMapping {
    EventSinkEnum target;
    int hwuiTrigger;
};

