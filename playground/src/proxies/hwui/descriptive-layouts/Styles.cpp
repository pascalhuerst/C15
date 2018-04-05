#include "Styles.h"
#include <proxies/hwui/HWUI.h>

namespace DescriptiveLayouts
{

  StyleSheet::StyleSheet()
  {

  }

  StyleSheet &StyleSheet::get()
  {
    static StyleSheet sheet;
    return sheet;
  }



}
