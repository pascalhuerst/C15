#include <Application.h>
#include <fstream>
#include <tools/StringTools.h>
#include "StyleParser.h"
#include "Styles.h"
#include "ControlRegistry.h"
#include "ControlParser.h"
#include <tuple>
#include <tools/json.h>
#include <device-settings/DebugLevel.h>
#include <boost/algorithm/string.hpp>
#include <tools/ExceptionTools.h>

using json = nlohmann::json;

namespace DescriptiveLayouts
{
  Rect parseRect(json rect)
  {
    try
    {
      std::string compact = rect;
      std::vector<std::string> splits;
      boost::split(splits, compact, boost::is_any_of(","));
      return Rect(std::stoi(splits[0]), std::stoi(splits[1]), std::stoi(splits[2]), std::stoi(splits[3]));
    }
    catch(...)
    {
      auto x = rect.at("X");
      auto y = rect.at("Y");
      auto w = rect.at("W");
      auto h = rect.at("H");
      return Rect(x, y, w, h);
    }
  }

   template <class T>
   T getFromJson(json j, std::string key, std::function<T(std::string)> converter = nullptr) {
     auto itProp = j.find(key);
     if(itProp != j.end()) {
       if(converter)
         return converter(*itProp);
       else
         return (T)*itProp;
     }
     return T{};
   }

  std::list<PrimitiveInstance> createPrimitives(json primitives)
  {
    std::list<PrimitiveInstance> lP;
    for(json::iterator primitive = primitives.begin(); primitive != primitives.end(); ++primitive) {
      auto key = primitive.key();
      auto value = primitive.value();

      auto primClass = getFromJson<PrimitiveClasses>(value, "Class", toPrimitiveClasses);
      auto prop = getFromJson<PrimitiveProperty>(value, "Property", toPrimitiveProperty);
      auto tag = getFromJson<PrimitiveTag>(value, "Tag");
      auto defaultText = getFromJson<DefaultText>(value, "Default");
      try {
        auto rect = parseRect(value.at("Rect"));
        lP.emplace_back(key, primClass, rect, tag, defaultText, prop);
      } catch(...) {
        auto e = std::current_exception();
        auto desc = ExceptionTools::handle_eptr(e);
        throw ExceptionTools::TemplateException(desc, "__LINE__ __FILE__");
      }

    }
    return lP;
  }

  std::list<ControlClass> createControls(json controlText)
  {
    std::list<ControlClass> l_cc;

    for(json::iterator critera = controlText.begin(); critera != controlText.end(); ++critera)
    {
      l_cc.push_back(ControlClass(critera.key(), createPrimitives(critera.value())));
    }

    return l_cc;
  }

  void registerControls(json j)
  {
    for(auto& c: createControls(j)) {
      ControlRegistry::get().registerControl(std::move(c));
    }
  }

  void importControls(const std::string &fileName)
  {
    DebugLevel::info("importing controls from file", fileName);
    std::ifstream i(fileName);
    json j;
    i >> j;

    auto it = j.find("controls");
    if(it != j.end())
    {
      json control = *it;
      registerControls(control);
    }
  }

}
