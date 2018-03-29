#pragma once

#include <proxies/hwui/HWUIEnums.h>
#include <functional>

namespace DescriptiveLayouts
{
  struct Selector
  {
      UIFocus m_focus;
      UIMode m_mode;
      std::function<bool()> m_detail;
  };
}
