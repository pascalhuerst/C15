#include "LayoutFactory.h"
#include "PrimitivesFactory.h"
#include "GenericLayout.h"
#include <Application.h>
#include <proxies/hwui/HWUI.h>
#include <tools/ExceptionTools.h>
#include "ControlClass.h"
#include "ControlInstance.h"
#include "Selector.h"
#include "TemplateEnums.h"
#include "EventSink.h"
#include "Styles.h"
#include "LayoutFolderMonitor.h"

namespace DescriptiveLayouts
{

  BoledLayoutFactory::BoledLayoutFactory()
  {
  }

  BoledLayoutFactory& BoledLayoutFactory::get()
  {
    static BoledLayoutFactory factory;
    return factory;
  }

  void BoledLayoutFactory::clear()
  {
    m_layouts.clear();
  }

  const DescriptiveLayouts::LayoutClass& BoledLayoutFactory::find(FocusAndMode fam) const
  {
    auto it = std::find_if(m_layouts.begin(), m_layouts.end(), [=](const LayoutClass& e)
    {
      return e.matches(fam) && e.meetsConditions();
    });

    if(it == m_layouts.end())
    {
      throw ExceptionTools::TemplateException("No matching layout found! current modes:" + [&]() -> Glib::ustring
      {
        Glib::ustring ret;
        ret += toString(fam.focus);
        ret += toString(fam.mode);
        ret += toString(fam.detail);
        return ret;
      }(), "__LINE__ __FILE__");
    }

    return *it;
  }

  std::shared_ptr<DFBLayout> BoledLayoutFactory::instantiate(FocusAndMode fam)
  {
    return std::shared_ptr<DFBLayout>(find(fam).instantiate());
  }

}
