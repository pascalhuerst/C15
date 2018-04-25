#include <fstream>
#include <tools/json.h>
#include "LayoutParser.h"
#include "LayoutFactory.h"

using json = nlohmann::json;

namespace DescriptiveLayouts {

    int toButton(std::string s) {
      return s == "BUTTON_INC" ? 0 : 1;
    }

    EventSinks toEventSinks(std::string s) {
      return s == "IncParam" ? EventSinks::IncParam : EventSinks::DecParam;
    }

    LayoutInstances toLayoutInstances(std::string name) {
      return LayoutInstances::UnmodulateableParameterLayout;
    }

    std::list<Selector> toSelectors(json j) {
      std::list<Selector> l;
      for(json::iterator selector = j.begin(); selector != j.end(); ++selector) {
        //ENUM KEY = selector.key();
        //ENUM VALUE = selector.value();
      }
      return l;
    }

    ControlInstances toControlInstances(json control) {
      return ControlInstances::Any;
    }


    ControlPrototype toControlPrototype(json control) {

      //return ControlPrototype;
    }

    Point toPoint(json control) {
      return Point(0,1);
    }
    EventSources toEventSources(json control) {
      return EventSources::ParameterDisplayString;
    }

    PrimitiveInstances toPrimitiveInstances(json control) {
      return PrimitiveInstances::Background;
    }

    LayoutPrototype::ControlInstanceList toControlInstanceList(json j) {
      LayoutPrototype::ControlInstanceList l;
      for(json::iterator control = j.begin(); control != j.end(); ++control) {
        ControlInstance(toControlInstances(*control), toControlPrototype(*control), toPoint(*control), toEventSources(*control), toPrimitiveInstances(*control));
      }
      return l;
    }

    LayoutPrototype::EventSinkList toEventSinkList(json j) {
      LayoutPrototype::EventSinkList l;
      for(json::iterator eventSink = j.begin(); eventSink != j.end(); ++eventSink) {
        l.push_back(EventSinkMapping(toButton(eventSink.key()), toEventSinks(eventSink.value())));
      }
      return l;
    }

    void parseLayout(json j) {

      for (json::iterator layout = j.begin(); layout != j.end(); ++layout) {
        auto name = layout.key();
        auto layoutContent = layout.value();
        auto selectorContent = *layoutContent.find("Selector");
        auto controlContent = *layoutContent.find("Controls");
        auto eventSinkContent = *layoutContent.find("EventSinks");


        LayoutInstances id = toLayoutInstances(name);
        std::list<Selector> selectors = toSelectors(selectorContent);
        LayoutPrototype::ControlInstanceList controls = toControlInstanceList(controlContent);
        LayoutPrototype::EventSinkList sinkMappings = toEventSinkList(eventSinkContent);
        LayoutPrototype p(id, selectors, controls, sinkMappings);
        BoledLayoutFactory::get().registerLayout(id, selectors, controls, sinkMappings);
      }
    }

    LayoutParser::LayoutParser() {
      std::ifstream i("/home/justus/development/C15/C15/playground/resources/Templates/layouts.lay");
      json j;
      i >> j;

      for (auto i: j) {
        parseLayout(i);
      }
    }
}