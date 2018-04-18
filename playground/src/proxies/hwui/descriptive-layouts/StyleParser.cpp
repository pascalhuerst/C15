#include <Application.h>
#include <fstream>
#include <tools/StringTools.h>
#include "StyleParser.h"
#include "Styles.h"

namespace DescriptiveLayouts
{
    const auto keywords = {"Style:", "Color:", "TextAlign:", "FontSize:", "BorderStyle:"};
    const auto delimiter = {',', ' ', '\t'};
    const auto newLine = {'\n', '\r'};

    std::string getFileContent(std::string path) {
      std::ifstream f(path);
      std::ostringstream ss;
      ss << f.rdbuf();
      return ss.str();
    }

    std::vector<std::string> getStyleParagraphs(std::string styleSheet)
    {
      return StringTools::splitStringOnStringDelimiter(styleSheet, "Style:");
    }

    std::string getValueToKey(std::string styleLine, std::string key)
    {
      styleLine.find(key, 0);
      return "";
    }
    std::tuple<UIFocus, UIMode, UIFocusAndModeDetail, LayoutInstances, ControlClasses, ControlInstances,
            PrimitiveClasses, PrimitiveInstances, StyleMap>
    createStyle(std::string styleParagraph)
    {
      auto lines = StringTools::splitStringOnAnyDelimiter(styleParagraph, '\n');
      auto selectors = lines[0];
      auto selector_v = StringTools::splitStringOnAnyDelimiter(selectors, ',');

      return {};

    }


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
      auto fileContent = getFileContent(Application::get().getResourcePath() + "/Templates/styles.tss");
      auto styles = getStyleParagraphs(fileContent);

      for (auto style: styles)
      {
        auto s = createStyle(style);
        //StyleSheet::get().registerStyle(std::get<0>(s), std::get<1>(s),
        //                                std::get<2>(s), std::get<3>(s),
        //                                std::get<4>(s), std::get<5>(s),
        //                                std::get<6>(s), std::get<7>(s),
        //                                std::get<8>(s));
      }
    }

}