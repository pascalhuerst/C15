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
    StyleSheet::StyleSelector parseSelector(json selector)
    {
      StyleSheet::StyleSelector theSelector;
      for (json::iterator critera = selector.begin(); critera != selector.end(); ++critera)
      {

      }
      return theSelector;
    }

    StyleMap parseStylePairs(json styles)
    {
      StyleMap theStyles;
      for (json::iterator style = styles.begin(); style != styles.end(); ++style)
      {

      }
      return theStyles;
    }

    void registerStyle(json::iterator style)
    {
      auto selector = parseSelector(*style.value().find("selector"));
      auto styles = parseStylePairs(*style.value().find("styles"));
      StyleSheet::get().registerStyle(selector, styles);
    }

    StyleParser::StyleParser()
    {
      std::ifstream i("/home/justus/development/C15/C15/playground/resources/Templates/styles.tss");
      json j;
      i >> j;
      for (json::iterator styles = j.begin(); styles != j.end(); ++styles) {
        for(auto style = styles.operator*().begin(); style != styles.operator*().end(); ++style){
          registerStyle(style);
        }
      }
    }

}