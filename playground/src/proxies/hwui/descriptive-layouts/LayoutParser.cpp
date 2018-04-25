#include <fstream>
#include <tools/json.h>
#include "LayoutParser.h"
#include "LayoutFactory.h"

using json = nlohmann::json;

namespace DescriptiveLayouts {

    int toButton(std::string s) {
      return s == "BUTTON_INC" ? 0 : 1;
    }

    Point toPoint(std::string s) {
      return s == "0,0" ? Point(0,0) : Point(99,99);
    }

    EventSinks toEventSinks(std::string s) {
      return s == "IncParam" ? EventSinks::IncParam : EventSinks::DecParam;
    }

    LayoutClasses toLayoutClasses(std::string name) {
      return LayoutClasses::UnmodulateableParameterLayout;
    }

    EventSources toEventSources(std::string s) {
      return s == "Slider" ? EventSources::SliderRange : EventSources::ParameterName;
    }

    std::list<Selector> toSelectors(json j) {
      std::list<Selector> l;
      for(json::iterator selector = j.begin(); selector != j.end(); ++selector) {
        //ENUM KEY = selector.key();
        //ENUM VALUE = selector.value();
      }
      return l;
    }

    ControlInstances toControlInstances(std::string s) {
      return ControlInstances::Any;
    }

    PrimitiveProperty toPrimitiveProperty(std::string s) {
      return s == "Text" ? PrimitiveProperty::Text : PrimitiveProperty::Range;
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

    LayoutClass::ControlInstanceList toControlInstanceList(json j) {
      LayoutClass::ControlInstanceList l;

      for(json::iterator control = j.begin(); control != j.end(); ++control) {
        auto controlInstances = toControlInstances(control.key());
        auto controlClasses = toControlClasses(*control.value().find("Class"));
        auto point = toPoint(*control.value().find("Position"));

        try {
          auto eventSources = toEventSources(*control.value().find("EventSource"));
          auto primitiveInstances = toPrimitiveInstances(*control.value().find("PrimitiveInstanceEventTarget"));

          l.emplace_back(controlInstances,controlClasses,point,eventSources,primitiveInstances);
        } catch(...) {
          l.emplace_back(controlInstances,controlClasses,point);
        }

      }
      return l;
    }

    LayoutClass::EventSinkList toEventSinkList(json j) {
      LayoutClass::EventSinkList l;
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


        auto id = toLayoutClasses(name);
        auto selectors = toSelectors(selectorContent);
        auto controls = toControlInstanceList(controlContent);
        auto sinkMappings = toEventSinkList(eventSinkContent);
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