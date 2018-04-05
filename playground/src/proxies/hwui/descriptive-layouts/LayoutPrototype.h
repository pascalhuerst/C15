#pragma once

#include <proxies/hwui/HWUIEnums.h>
#include "Selector.h"
#include "EventSink.h"

class DFBLayout;

namespace DescriptiveLayouts
{

  class ControlInstance;

  struct LayoutPrototype
  {
    public:
      using ControlInstanceList = std::list<ControlInstance>;
      using EventSinkList = std::list<EventSinkMapping>;

      template<typename ... Args>
        LayoutPrototype(LayoutInstances id, Args ... args) :
            id(id)
        {
          (void) std::initializer_list<int> { (addToList(args), 0)... };
        }

      bool matches(FocusAndMode fam) const;

      DFBLayout* instantiate() const;

    private:
      void addToList(Selector s);
      void addToList(ControlInstance s);
      void addToList(EventSinkMapping s);

      LayoutInstances id;
      std::list<Selector> selectors;
      ControlInstanceList controls;
      EventSinkList sinkMappings;

      friend class GenericLayout;
  };

}
