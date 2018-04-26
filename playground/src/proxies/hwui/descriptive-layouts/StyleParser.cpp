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

    try {
      theSelector.f = toUIFocus(selector.at("UIFocus"));
    } catch(...) {}
    try {
      theSelector.m = toUIMode(selector.at("UIMode"));
    } catch(...) {}
    try {
      theSelector.d = toUIFocusAndModeDetail(selector.at("UIFocusAndModeDetail"));
    } catch(...) {}
    try {
      theSelector.l = toLayoutClasses(selector.at("LayoutClasses"));
    } catch(...) {}
    try {
      theSelector.cc = toControlClasses(selector.at("ControlClasses"));
    } catch(...) {}
    try {
      theSelector.ci = toControlInstances(selector.at("ControlInstances"));
    } catch(...) {}
    try {
      theSelector.pc = toPrimitiveClasses(selector.at("PrimitiveClasses"));
    } catch(...) {}
    try {
      theSelector.pi = toPrimitiveInstances(selector.at("PrimitiveInstances"));
    } catch(...) {}

    return theSelector;
  }

  StyleMap parseStylePairs(json styles)
  {
    StyleMap theStyles;
    for(json::iterator style = styles.begin(); style != styles.end(); ++style)
    {
      auto styleKey = toStyleKey(style.key());

      switch(styleKey) {
        case StyleKey::BorderStyle:
          theStyles.emplace(styleKey, (int)StyleValues::toBorderStyle(style.value()));
          break;
        case StyleKey::BackgroundColor:
          theStyles.emplace(styleKey, (int)StyleValues::toColor(style.value()));
          break;
        case StyleKey::Color:
          theStyles.emplace(styleKey, (int)StyleValues::toColor(style.value()));
          break;
        case StyleKey::FontSize:
          theStyles.emplace(styleKey, (int)StyleValues::toFont(style.value()));
          break;
        case StyleKey::TextAlign:
          theStyles.emplace(styleKey, (int)StyleValues::toAlignment(style.value()));
          break;
      }
    }
    return theStyles;
  }

  void registerStyle(json style)
  {
    try
    {
      auto selector = parseSelector(style.at("selector"));
      auto styles = parseStylePairs(style.at("styles"));
      StyleSheet::get().registerStyle(selector, styles);
    }
    catch(std::exception e)
    {
      DebugLevel::error(to_string("JSON Parse Error: ") + e.what());
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
