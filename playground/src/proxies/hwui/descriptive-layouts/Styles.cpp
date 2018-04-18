#include "Styles.h"
#include "Styleable.h"
#include <proxies/hwui/HWUI.h>

namespace DescriptiveLayouts
{

  StyleSheet::StyleSheet()
  {

  }

  void StyleSheet::registerStyleFull(UIFocus f, UIMode m, UIFocusAndModeDetail d, LayoutInstances l, ControlClasses cc, ControlInstances ci,
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
