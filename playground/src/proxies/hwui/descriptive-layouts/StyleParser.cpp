#include <Application.h>
#include <fstream>
#include <tools/StringTools.h>
#include "StyleParser.h"
#include "Styles.h"
#include <tuple>
#include <tools/json.h>

using json = nlohmann::json;

namespace DescriptiveLayouts
{
    /*
     * UIFocus f,
     * UIMode m,
     * UIFocusAndModeDetail d,
     * LayoutInstances l,
     * ControlClasses cc,
     * ControlInstances ci,
     * PrimitiveClasses pc,
     * PrimitiveInstances pi,
     * const StyleMap &s <- map (string, int)
     */

    StyleParser::StyleParser()
    {
      std::ifstream i("/home/justus/development/C15/C15/playground/resources/Templates/styles.tss");
      json j;
      i >> j;
      for (json::iterator it = j.begin(); it != j.end(); ++it) {
        std::cout << it.key() << " : " << it.value() << "\n";
      }
    }

}