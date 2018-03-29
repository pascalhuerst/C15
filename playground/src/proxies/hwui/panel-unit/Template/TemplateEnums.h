#pragma once

#include <tuple>
#include <proxies/hwui/controls/Rect.h>

class TemplateElement;

namespace UI {


  enum class Components {
    Label,
    Bar
  };

  enum class ComponentValues {
      text,
      length
  };

  enum class Style {
    ForegroundColor,
    BackgroundColor,
    TextAlign,
    FontSize,
    BorderStyle,
    BorderWidth
  };

  namespace StyleValues {
    enum class Font {
      Bold, Regular
    };

    enum class Color {
      C43, C77, C103, C128, C179, C204, C255, Transparent
    };

    enum class Alignment {
      Left, Center, Right
    };

    enum class BorderStyle {
      Dotted, Solid, Rounded, None
    };
  };


  struct TemplateElement {
      const char* id;
      Components c;
      Rect pos;
      std::initializer_list<std::pair<Style, int>> style;
  };

};

