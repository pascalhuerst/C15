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
  template<class T>
    bool readFieldFromJson(json j, Glib::ustring key, std::function<T(std::string)> converter, std::list<Selector>& outList)
    {
      auto it = j.find(key);
      if(it != j.end())
      {
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
    if(pt.is_string())
    {
      std::string compact = pt;
      std::vector<std::string> splits;
      boost::split(splits, compact, boost::is_any_of(","));
      return Point(std::stoi(splits[0]), std::stoi(splits[1]));
    }
    else
    {
      auto x = pt.at("X");
      auto y = pt.at("Y");
      return Point(x, y);
    }
  }

  ControlInstance::EventConnection parseEventConnection(const std::string &srcString, const std::string &instString, const std::string &propString)
  {
    auto src = toEventSources(boost::trim_copy(srcString));
    auto inst = boost::trim_copy(instString);
    auto prop = toPrimitiveProperty(boost::trim_copy(propString));
    return { src, inst, prop };
  }

  ControlInstance::EventConnection stringToEventConnection(const std::string& s)
  {
    static std::regex reg("(.*)=>(.*)\\[(.*)\\]");
    std::smatch m;

    if(std::regex_search(s, m, reg))
    {
      try
      {
        return parseEventConnection(m[1], m[2], m[3]);
      }
      catch(const std::exception& e)
      {
        DebugLevel::throwException("Could not parse 'Events' property:", s, "(", e.what(), ")");
      }
    }
    else
    {
      DebugLevel::throwException("Could not parse 'Events' property:", s);
    }
    return {};
  }

  ControlInstance::EventConnections parseEventConnections(json j)
  {
    ControlInstance::EventConnections ret;
    auto it = j.find("Events");

    if(it != j.end())
    {
      std::string str = *it;
      std::list<std::string> connections;
      boost::split(connections, str, boost::is_any_of(","));
      std::transform(connections.begin(),  connections.end(), std::back_inserter(ret), stringToEventConnection);
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
