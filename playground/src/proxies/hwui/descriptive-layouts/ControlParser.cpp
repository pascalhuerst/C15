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
  Rect parseRect(std::string rect)
  {
    auto r = StringTools::splitStringOnAnyDelimiter(rect, ',');
    return Rect(std::stoi(r[0]), std::stoi(r[1]), std::stoi(r[2]), std::stoi(r[3]));
  }

  std::list<PrimitiveInstance> createPrimitives(json primitives)
  {
    std::list<PrimitiveInstance> lP;
    for(json::iterator primitive = primitives.begin(); primitive != primitives.end(); ++primitive)
    {
      auto key = primitive.key();
      auto value = primitive.value();

      PrimitiveProperty prop = PrimitiveProperty::None;
      auto itProp = value.find("Property");

      if(itProp != value.end())
        prop = toPrimitiveProperty(*itProp);

      lP.emplace_back(toPrimitiveInstances(key), toPrimitiveClasses(value.at("Class")), parseRect(value.at("Rect")), prop);
    }
    return lP;
  }

  void createControls(json controlText)
  {
    for(json::iterator critera = controlText.begin(); critera != controlText.end(); ++critera)
    {
      auto name = critera.key();
      auto primitiveList = critera.value();
      ControlRegistry::get().registerControl(ControlClass(toControlClasses(name), createPrimitives(primitiveList)));
    }
  }

  void registerControls(json j)
  {
    try
    {
      createControls(j);
    }
    catch(...)
    {
      DebugLevel::error("JSON NOT OKAY! ok?");
    }
  }

  void importControls(const std::string &fileName)
  {
    try
    {
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
