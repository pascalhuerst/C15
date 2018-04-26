#include <Application.h>
#include <fstream>
#include <tools/StringTools.h>
#include "StyleParser.h"
#include "Styles.h"
#include <tuple>
#include <tools/json.h>
#include <device-settings/DebugLevel.h>

using json = nlohmann::json;

namespace DescriptiveLayouts
{
  StyleSheet::StyleSelector parseSelector(json selector)
  {
    StyleSheet::StyleSelector theSelector;
    for(json::iterator critera = selector.begin(); critera != selector.end(); ++critera)
    {

    }
    return theSelector;
  }

  StyleMap parseStylePairs(json styles)
  {
    StyleMap theStyles;
    for(json::iterator style = styles.begin(); style != styles.end(); ++style)
    {

    }
    return theStyles;
  }

  void registerStyle(json style)
  {
    try
    {
      auto selector = parseSelector(*style.find("selector"));
      auto styles = parseStylePairs(*style.find("styles"));
      StyleSheet::get().registerStyle(selector, styles);
    }
    catch(...)
    {
      DebugLevel::error("JSON NOT OKAY! ok?");
    }
  }

  void importStyles(const std::string &fileName)
  {
    try
    {
      std::ifstream i(fileName);
      json j;
      i >> j;
      auto styles = j.at("styles");

      for(auto s : styles)
        registerStyle(s);
    }
    catch(...)
    {
    }
  }

}
