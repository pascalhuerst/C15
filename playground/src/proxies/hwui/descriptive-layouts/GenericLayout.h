#pragma once

#include <proxies/hwui/DFBLayout.h>
#include "LayoutFactory.h"

namespace DescriptiveLayouts
{
  class PropertyOwner;

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
      void onEventSourceFired(std::any value, const EventSourceMapping &e);

      std::list<Template> m_templates;
      std::map<std::string, PropertyOwner*> m_children;
      std::list<sigc::connection> m_connections;
  };
}
