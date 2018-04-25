#pragma once

#include <cstring>
#include <assert.h>
#include <list>
#include <proxies/hwui/DFBLayout.h>
#include <proxies/hwui/FrameBuffer.h>
#include <proxies/hwui/controls/Rect.h>
#include "GenericLayout.h"
#include "EventSink.h"
#include "EventSource.h"
#include "Selector.h"
#include "LayoutPrototype.h"

namespace DescriptiveLayouts
{
  class GenericControl;

  class BoledLayoutFactory
  {
    public:
      static BoledLayoutFactory& get();

      std::shared_ptr<DFBLayout> instantiate(FocusAndMode fam);

      template<typename ... Args>
      void registerLayout(LayoutInstances id, Args ... args)
      {
        m_layouts.emplace_back(id, args...);
      }

      void registerLayout(LayoutInstances id,
                          std::list<Selector> sel,
                          std::list<ControlInstance> ci,
                          std::list<EventSinkMapping> esm) {
        m_layouts.emplace_back(id, sel, ci, esm);
      }

    private:
      BoledLayoutFactory();



      const LayoutPrototype& find(FocusAndMode fam) const;

      std::list<LayoutPrototype> m_layouts;
  };
}
