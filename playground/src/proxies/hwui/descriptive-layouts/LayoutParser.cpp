#include <fstream>
#include <Application.h>
#include <Options.h>
#include <tools/json.h>
#include "LayoutParser.h"
#include "LayoutFactory.h"

using json = nlohmann::json;

namespace DescriptiveLayouts
{
  template<class T>
  bool readFieldFromJson(json j, Glib::ustring key, std::function<T(std::string)> converter, std::list<Selector>& outList) {
    auto it = j.find(key);
    if(it != j.end()) {
      T x = converter(*it);
      outList.push_back(x);
      return true;
    }
    return false;
  }

  std::list<Selector> toSelectors(json selector)
  {
    std::list<Selector> selectors;

    if(!readFieldFromJson<UIFocus>(selector, "UIFocus", toUIFocus, selectors))
      selectors.push_back(UIFocus::Any);
    if(!readFieldFromJson<UIMode>(selector, "UIMode", toUIMode, selectors))
      selectors.push_back(UIMode::Any);
    if(readFieldFromJson<UIFocusAndModeDetail>(selector, "UIFocusAndModeDetail", toUIFocusAndModeDetail, selectors))
      selectors.push_back(UIFocusAndModeDetail::Any);

    return selectors;
  }

  Point toPoint(json pt)
  {
    try
    {
      std::string compact = pt;
      std::vector<std::string> splits;
      boost::split(splits, compact, boost::is_any_of(","));
      return Point(std::stoi(splits[0]), std::stoi(splits[1]));
    }
    catch(...)
    {
      auto x = pt.at("X");
      auto y = pt.at("Y");
      return Point(x, y);
    }
  }

  LayoutClass::ControlInstanceList toControlInstanceList(json j)
  {
    LayoutClass::ControlInstanceList l;

    for(json::iterator control = j.begin(); control != j.end(); ++control)
    {
      auto controlInstances = control.key();
      auto controlClasses = control.value().at("Class");
      auto point = toPoint(control.value().at("Position"));

      try
      {
        auto eventSources = toEventSources(control.value().at("EventSource"));
        PrimitiveInstances primitiveInstances = control.value().at("PrimitiveInstanceEventTarget");

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
      l.push_back(EventSinkMapping(toButtons(eventSink.key()), toEventSinks(eventSink.value())));
    }
    return l;
  }

  void parseLayout(json j)
  {
    for(json::iterator layout = j.begin(); layout != j.end(); ++layout)
    {
      auto name = layout.key();

      DebugLevel::info("importing layout", name);

      auto layoutContent = layout.value();
      auto selectorContent = layoutContent.at("Selector");
      auto controlContent = layoutContent.at("Controls");
      auto eventSinkContent = layoutContent.at("EventSinks");

      auto id = name;
      auto selectors = toSelectors(selectorContent);
      auto controls = toControlInstanceList(controlContent);
      auto sinkMappings = toEventSinkList(eventSinkContent);
      BoledLayoutFactory::get().registerLayout(id, selectors, controls, sinkMappings);
    }
  }

  void importLayout(const std::string &fileName)
  {
    DebugLevel::info("importing layouts from file", fileName);
    std::ifstream i(fileName);
    json j;
    i >> j;

    auto it = j.find("layouts");
    if(it != j.end())
    {
      json layouts = *it;
      parseLayout(layouts);
    }
  }

}
