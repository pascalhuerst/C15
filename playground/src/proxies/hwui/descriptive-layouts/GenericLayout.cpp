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

  void GenericLayout::onInit()
  {
    super::onInit();
    addElements();
    connectEventSources();
  }

  void GenericLayout::addElements()
  {
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

  void GenericLayout::connectEventSources()
  {
    for(auto &t : m_templates)
    {
      for(auto &e : t.sourceMapping)
      {
        connectEventSource(e);
      }
    }
  }

  void GenericLayout::connectEventSource(const EventSourceMapping &e)
  {
    auto memFun = sigc::mem_fun(this, &GenericLayout::onEventSourceFired);
    auto withBoundMapping = sigc::bind<0>(memFun, e);
    EventSourceBroker::get().connect(e.source, withBoundMapping);
  }

  void GenericLayout::onEventSourceFired(const EventSourceMapping &e, std::any value)
  {
    m_children.at(e.targetID)->setProperty(e.valueTarget, value);
  }
}
