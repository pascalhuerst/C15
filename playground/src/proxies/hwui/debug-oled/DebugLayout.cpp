#include <proxies/hwui/controls/Label.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/MultiLineLabel.h>
#include "DebugLayout.h"

DebugLayout::DebugLayout(Glib::ustring e) {
  super();

  DebugLevel::warning("Installing Debug Layout!\n" + e);

  addControl(new Label("Debug!", Rect(0, 0, 125, 10)));

  auto text = addControl(new MultiLineLabel(e));
  text->setPosition(Rect(0, 10, 125, 90));
}