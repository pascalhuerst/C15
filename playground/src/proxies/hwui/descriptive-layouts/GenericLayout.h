#pragma once

#include <proxies/hwui/DFBLayout.h>
#include "LayoutFactory.h"

namespace DescriptiveLayouts
{
  class GenericLayout : public DFBLayout
  {
      using super = DFBLayout;

    public:
      GenericLayout(std::initializer_list<Template> templates, OLEDProxy &oled);
      virtual ~GenericLayout();

    protected:
      void onInit() override;

    private:
      void addElement(const TemplateElement &e);

      std::initializer_list<Template> m_templates;
      std::map<std::string, Control*> m_children;
  };
}
