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
    for(auto &e : m_connections)
      e.disconnect();
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
    m_connections.push_back(EventSourceBroker::get().connect(e.source, sigc::bind(sigc::mem_fun(this, &GenericLayout::onEventSourceFired), e)));
  }

  void GenericLayout::onEventSourceFired(std::any value, const EventSourceMapping &e)
  {
    m_children.at(e.targetID)->setProperty(e.valueTarget, e.converter ? e.converter(value) : value);
  }
}
