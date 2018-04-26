#include <fstream>
#include <Application.h>
#include <Options.h>
#include <tools/json.h>
#include "LayoutParser.h"
#include "LayoutFactory.h"

using json = nlohmann::json;

namespace DescriptiveLayouts
{
  int toButton(std::string s)
  {
    return s == "BUTTON_INC" ? 0 : 1;
  }

  Point toPoint(std::string s)
  {
    return s == "0,0" ? Point(0, 0) : Point(99, 99);
  }

  std::list<Selector> toSelectors(json j)
  {
    std::list<Selector> l;
    for(json::iterator selector = j.begin(); selector != j.end(); ++selector)
    {
      //ENUM KEY = selector.key();
      //ENUM VALUE = selector.value();
    }
    return l;
  }

  Point toPoint(json control)
  {
    return Point(0, 1);
  }

  LayoutClass::ControlInstanceList toControlInstanceList(json j)
  {
    LayoutClass::ControlInstanceList l;

    for(json::iterator control = j.begin(); control != j.end(); ++control)
    {
      auto controlInstances = toControlInstances(control.key());
      auto controlClasses = toControlClasses(control.value().at("Class"));
      auto point = toPoint(control.value().at("Position"));

      try
      {

        auto eventSources = toEventSources(control.value().at("EventSource"));
        auto primitiveInstances = toPrimitiveInstances(control.value().at("PrimitiveInstanceEventTarget"));

        l.emplace_back(controlInstances, controlClasses, point, eventSources, primitiveInstances);
      }
      catch(...)
      {
        l.emplace_back(controlInstances, controlClasses, point);
      }

    }
    return l;
  }

  LayoutClass::EventSinkList toEventSinkList(json j)
  {
    LayoutClass::EventSinkList l;
    for(json::iterator eventSink = j.begin(); eventSink != j.end(); ++eventSink)
    {
      l.push_back(EventSinkMapping(toButton(eventSink.key()), toEventSinks(eventSink.value())));
    }
    return l;
  }

  void parseLayout(json j)
  {
    for(json::iterator layout = j.begin(); layout != j.end(); ++layout)
    {
      auto name = layout.key();
      auto layoutContent = layout.value();
      auto selectorContent = layoutContent.at("Selector");
      auto controlContent = layoutContent.at("Controls");
      auto eventSinkContent = layoutContent.at("EventSinks");

      auto id = toLayoutClasses(name);
      auto selectors = toSelectors(selectorContent);
      auto controls = toControlInstanceList(controlContent);
      auto sinkMappings = toEventSinkList(eventSinkContent);
      BoledLayoutFactory::get().registerLayout(id, selectors, controls, sinkMappings);
    }
  }

  void importLayout(const std::string &fileName)
  {
    try
    {
      std::ifstream i(fileName);
      json j;
      i >> j;
      parseLayout(j.at("layouts"));
    }
    catch(...)
    {
    }
  }

}
