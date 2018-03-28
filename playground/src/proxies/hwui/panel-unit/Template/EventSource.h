#pragma once

#include <sigc++/connection.h>
#include <tools/Signal.h>
#include <any>
#include "TemplateEnums.h"



struct EventSource {
    sigc::connection m_inputConnection;
    Signal<void, std::any> m_outputSignal;
};

enum class EventSourceEnum {
    parameterName,
    parameterValue,
    parameterDisplayString,
    paramterGroupName
};

class EventSourceBroker {
private:
    EventSourceBroker() {
      //TODO fill me pls m_map
    };
public:
    static EventSourceBroker& get() {
      static EventSourceBroker s;
      return s;
    }

    std::unordered_map<EventSourceEnum, EventSource> m_map;
};

struct EventSourceMapping {
    EventSourceEnum source;
    const char* targetID;
    UI::ComponentValues valueTarget;
    std::function<std::any(std::any)> converter;
};