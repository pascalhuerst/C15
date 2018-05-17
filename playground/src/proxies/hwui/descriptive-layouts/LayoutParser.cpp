#include <fstream>
#include <Application.h>
#include <Options.h>
#include <tools/json.h>
#include "LayoutParser.h"
#include "LayoutFactory.h"
#include <boost/algorithm/string.hpp>
#include <regex>

using json = nlohmann::json;

namespace DescriptiveLayouts
{
  std::list<Selector> toSelectors(json selector)
  {
    std::list<Selector> selectors;

    try
    {
      selectors.push_back(toUIFocus(selector.at("UIFocus")));
    }
    catch(...)
    {
      selectors.push_back(UIFocus::Any);
    }
    try
    {
      selectors.push_back(toUIMode(selector.at("UIMode")));
    }
    catch(...)
    {
      selectors.push_back(UIMode::Any);
    }
    try
    {
      selectors.push_back(toUIFocusAndModeDetail(selector.at("UIFocusAndModeDetail")));
    }
    catch(...)
    {
      selectors.push_back(UIFocusAndModeDetail::Any);
    }

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

  ControlInstance::EventConnections parseEventConnections(json j)
  {
    ControlInstance::EventConnections ret;

    try
    {
      std::string str = j.at("Events");
      std::list<std::string> connections;
      boost::split(connections, str, boost::is_any_of(","));

      static std::regex reg("(.*)=>(.*)\\[(.*)\\]");

      for(auto s : connections)
      {
        std::smatch m;

        while(std::regex_search(s, m, reg))
        {
          if(m.size() >= 4)
          {
            std::string srcString = m[1];
            std::string instString = m[2];
            std::string propString = m[3];
            auto src = toEventSources(boost::trim_copy(srcString));
            auto inst = boost::trim_copy(instString);
            auto prop = toPrimitiveProperty(boost::trim_copy(propString));
            ret.push_back({src, inst, prop});

          }
          s = m.suffix().str();
        }

      }
    }
    catch(...)
    {
    }
    return ret;
  }

  LayoutClass::ControlInstanceList toControlInstanceList(json j)
  {
    LayoutClass::ControlInstanceList l;

    for(json::iterator control = j.begin(); control != j.end(); ++control)
    {
      auto controlInstances = control.key();
      auto controlClasses = control.value().at("Class");
      auto point = toPoint(control.value().at("Position"));
      auto eventConnections = parseEventConnections(control.value());
      l.emplace_back(controlInstances, controlClasses, point, eventConnections);
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
    try
    {
      DebugLevel::info("importing layouts from file", fileName);
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
