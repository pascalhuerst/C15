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
    PrimitiveInstances giveEnum(std::string value) {
      return PrimitiveInstances::Background;
    }

    Rect parseRect(std::string rect) {
      auto r = StringTools::splitStringOnAnyDelimiter(rect, ',');
      return Rect(std::stoi(r[0]), std::stoi(r[1]), std::stoi(r[2]), std::stoi(r[3]));
    }


    std::list<PrimitiveInstance> createPrimitives(json primitives)
    {
      std::list<PrimitiveInstance> lP;
      for(json::iterator primitive = primitives.begin(); primitive != primitives.end(); ++primitive) {
        //giveEnum(primitive.value().find("Class"))
        //giveEnum(primitive.value().find("Property"))
        lP.push_back(PrimitiveInstance(giveEnum(primitive.key()),
                               PrimitiveClasses::Bar,
                               parseRect(*primitive.value().find("Rect")),
                               PrimitiveProperty::None));
      }
      return lP;
    }

    void createControls(json controlText)
    {
      for (json::iterator critera = controlText.begin(); critera != controlText.end(); ++critera)
      {
          auto name = critera.key();
          std::cout << name << '\n';
          auto primitiveList = critera.value();

          ControlClass controlPrototype((ControlClasses)giveEnum(name), createPrimitives(primitiveList));
          ControlRegistry::get().registerControl(name, controlPrototype);
      }
    }

    void registerControls(json j)
    {
      try {
        createControls(j);
      } catch(...) {
        DebugLevel::error("JSON NOT OKAY! ok?");
      }
    }

    ControlParser::ControlParser()
    {
      std::ifstream i("/home/justus/development/C15/C15/playground/resources/Templates/controls.ctrls");
      json j; i >> j;

      for(auto i: j) {
        registerControls(i);
      }
    }

}