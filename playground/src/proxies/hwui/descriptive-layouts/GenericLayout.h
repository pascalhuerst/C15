#pragma once

#include <proxies/hwui/DFBLayout.h>
#include "LayoutFactory.h"

namespace DescriptiveLayouts
{
  class BasicBuildingBlock;

  class GenericLayout : public DFBLayout
  {
      using super = DFBLayout;

    public:
      GenericLayout(std::initializer_list<Template> templates, OLEDProxy &oled);
      virtual ~GenericLayout();

    protected:
      void onInit() override;

    private:
      void addElements();
      void addElement(const TemplateElement &e);
      void connectEventSources();
      void connectEventSource(const EventSourceMapping &e);
      void onEventSourceFired(const EventSourceMapping &e, std::any value);

      std::list<Template> m_templates;
      std::map<std::string, BasicBuildingBlock*> m_children;
  };
}
