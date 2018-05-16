#include <proxies/hwui/controls/Label.h>
#include "DebugLayout.h"

DebugLayout::DebugLayout(std::exception e) : m_exception(e) {
  super();
  addControl(new Label(e.what(), Rect(0,0,124,14)));
}