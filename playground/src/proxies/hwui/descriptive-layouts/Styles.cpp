#include "Styles.h"
#include "Styleable.h"
#include <proxies/hwui/HWUI.h>

namespace DescriptiveLayouts
{

  StyleSheet::StyleSheet()
  {
    registerStyle(UIFocus::Any, UIMode::Any, UIFocusAndModeDetail::Any,
                  LayoutInstances::Any, ControlClasses::Any, ControlInstances::Any,
                  PrimitiveClasses::Bar, PrimitiveInstances::Background,
    {
      { StyleKey::Color, (int)StyleValues::Color::C43}
    });

    registerStyle(UIFocus::Any, UIMode::Any, UIFocusAndModeDetail::Any,
                  LayoutInstances::Any, ControlClasses::Any, ControlInstances::Any,
                  PrimitiveClasses::Text, PrimitiveInstances::Any,
    {
      { StyleKey::TextAlign, (int) StyleValues::Alignment::Center},
      { StyleKey::FontSize, 9 },
      { StyleKey::Color, (int) FrameBuffer::C255 }
    });
  }

  void StyleSheet::registerStyle(UIFocus f, UIMode m, UIFocusAndModeDetail d, LayoutInstances l, ControlClasses cc, ControlInstances ci,
                                 PrimitiveClasses pc, PrimitiveInstances pi, const StyleMap &s)
  {
    m_styles[f][m][d][l][cc][ci][pc][pi] = s;
  }

  void StyleSheet::applyStyle(UIFocus f, UIMode m, UIFocusAndModeDetail d, LayoutInstances l, ControlClasses cc, ControlInstances ci,
                              PrimitiveClasses pc, PrimitiveInstances pi, Styleable *target) const
  {
    StyleMap style;
    m_styles.collectStyle(style, f, m, d, l, cc, ci, pc, pi);
    target->applyStyle(style);
  }

  StyleSheet &StyleSheet::get()
  {
    static StyleSheet sheet;
    return sheet;
  }

}
