#include "LayoutPrototype.h"
#include "ControlInstance.h"
#include "GenericLayout.h"

namespace DescriptiveLayouts
{

  void LayoutPrototype::addToList(EventSinkMapping s)
  {
    sinkMappings.push_back(s);
  }

  void LayoutPrototype::addToList(ControlInstance s)
  {
    controls.push_back(s);
  }

  void LayoutPrototype::addToList(Selector s)
  {
    selectors.push_back(s);
  }

  DFBLayout* LayoutPrototype::instantiate() const
  {
    return new GenericLayout(*this);
  }

  bool LayoutPrototype::matches(FocusAndMode fam) const
  {
    return std::all_of(selectors.begin(), selectors.end(), [=](const Selector& s)
    {
      return s.test(fam);
    });
  }
}
