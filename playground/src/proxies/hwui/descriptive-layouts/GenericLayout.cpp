#include "GenericLayout.h"
#include "BasicBuildingBlock.h"

namespace DescriptiveLayouts
{

  GenericLayout::GenericLayout(std::initializer_list<Template> templates, OLEDProxy &oled) :
      super(oled),
      m_templates(templates)
  {
  }

  GenericLayout::~GenericLayout()
  {
  }

  void GenericLayout::onInit ()
  {
    super::onInit();

    for(auto &t : m_templates)
    {
      for(auto &e : t.elements)
      {
        addElement(e);
      }
    }
  }

  void GenericLayout::addElement(const TemplateElement &e)
  {
    if(e.c == Components::BasicBuildingBlock)
    {
      m_children[e.id] = addControl(new BasicBuildingBlock(e));
    }
  }
}
