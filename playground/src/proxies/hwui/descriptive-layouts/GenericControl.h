#pragma once

#include "ControlInstance.h"
#include "proxies/hwui/controls/ControlWithChildren.h"

namespace DescriptiveLayouts
{
  class GenericControl : public ControlWithChildren
  {
    public:
      GenericControl(const ControlInstance &prototype);
      virtual ~GenericControl();

      void style(LayoutClasses layout);
      void connect();

    private:
      void addPrimitives();
      void onEventFired(std::any v);

      ControlInstance m_prototype;
      sigc::connection m_connection;
  };

}
