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

  std::list<PrimitiveInstance> createPrimitives(json primitives)
  {
    std::list<PrimitiveInstance> lP;
    for(json::iterator primitive = primitives.begin(); primitive != primitives.end(); ++primitive)
    {
      auto key = primitive.key();
      auto value = primitive.value();

      DebugLevel::info("add primitve", key);

      PrimitiveProperty prop = PrimitiveProperty::None;
      auto itProp = value.find("Property");

      if(itProp != value.end())
        prop = toPrimitiveProperty(*itProp);

      PrimitiveTag tag = PrimitiveTag::None;
      auto itTag = value.find("Tag");

      if(itTag != value.end())
        tag = *itTag;

      lP.emplace_back(key, toPrimitiveClasses(value.at("Class")), parseRect(value.at("Rect")), tag, prop);
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
