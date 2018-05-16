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

using json = nlohmann::json;

namespace DescriptiveLayouts
{
  Rect parseRect(json rect)
  {
    auto x = rect.at("X");
    auto y = rect.at("Y");
    auto w = rect.at("W");
    auto h = rect.at("H");

    return Rect(x,y,w,h);
  }

   template <class T>
   T getFromJson(json j, std::string key, std::function<T(std::string)> converter = nullptr) {
     T property;
     auto itProp = j.find(key);

     if(itProp != j.end()) {
       if(converter != nullptr)
         return converter(*itProp);
       else
         return *itProp;
     }
     return T{};
   }

  std::list<PrimitiveInstance> createPrimitives(json primitives)
  {
    std::list<PrimitiveInstance> lP;
    for(json::iterator primitive = primitives.begin(); primitive != primitives.end(); ++primitive) {
      auto key = primitive.key();
      auto value = primitive.value();

      try {
        auto primClass = getFromJson<PrimitiveClasses>(value, "Class", [](std::string u){return toPrimitiveClasses(u);});
        auto prop = getFromJson<PrimitiveProperty>(value, "Property", [](std::string u){return toPrimitiveProperty(u);});
        auto tag = getFromJson<PrimitiveTag>(value, "Tag");
        auto defaultText = getFromJson<DefaultText>(value, "Default");
        auto rect = parseRect(value.at("Rect"));
        lP.emplace_back(key, primClass, rect, tag, defaultText, prop);
      } catch(std::runtime_error e) {
        DebugLevel::warning("Could not parse Control!\n"s + e.what());
      }
    }
    return lP;
  }

  void createControls(json controlText)
  {
    for(json::iterator critera = controlText.begin(); critera != controlText.end(); ++critera)
    {
      auto name = critera.key();
      auto primitiveList = critera.value();
      DebugLevel::info("register control", name);
      ControlRegistry::get().registerControl(ControlClass(name, createPrimitives(primitiveList)));
    }
  }

  void registerControls(json j)
  {
    try
    {
      createControls(j);
    }
    catch(std::exception e)
    {
      DebugLevel::error(to_string("JSON Parsing Error: ") + e.what());
    }
  }

  void importControls(const std::string &fileName)
  {
    try
    {
      DebugLevel::info("importing controls from file", fileName);
      std::ifstream i(fileName);
      json j;
      i >> j;
      registerControls(j.at("controls"));
    }
    catch(...)
    {
    }
  }

}
