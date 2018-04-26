#include "GenericControl.h"

#include <tools/EnumTools.h>
#include "Styleable.h"
#include "EventSource.h"
#include "PropertyOwner.h"
#include "ControlRegistry.h"

namespace DescriptiveLayouts
{
  GenericControl::GenericControl(const ControlInstance &prototype) :
      ControlWithChildren(Rect(prototype.position, Point(0, 0))),
      m_prototype(prototype)
  {
    addPrimitives();
  }

  GenericControl::~GenericControl()
  {
    m_connection.disconnect();
  }

  void GenericControl::addPrimitives()
  {
    auto rect = getPosition();

    int maxX = 0;
    int maxY = 0;

    const auto &controlClass = ControlRegistry::get().find(m_prototype.controlClass);

    for(auto &p : controlClass.primitves)
    {
      auto c = addControl(p.instantiate());
      maxX = std::max(maxX, c->getPosition().getRight());
      maxY = std::max(maxY, c->getPosition().getBottom());
    }

    rect.setWidth(maxX + 1);
    rect.setHeight(maxY + 1);
    setPosition(rect);
  }

  void GenericControl::style(LayoutClasses layout)
  {
    for(auto &p : getControls())
    {
      if(auto a = dynamic_pointer_cast<Styleable>(p))
      {
        a->style(layout, m_prototype.controlClass, m_prototype.controlInstance);
      }
    }
  }

  void GenericControl::connect()
  {
    m_connection = EventSourceBroker::get().connect(m_prototype.eventSource, sigc::mem_fun(this, &GenericControl::onEventFired));
  }

  void GenericControl::onEventFired(std::any v)
  {
    for(auto c : getControls())
    {
      if(auto a = dynamic_pointer_cast<Styleable>(c))
      {
        const auto &primitive = a->getPrimitive();

        if(primitive.primitiveInstance == m_prototype.eventTarget)
        {
          if(auto p = dynamic_pointer_cast<PropertyOwner>(c))
          {
            p->setProperty(primitive.eventTarget, v);
          }

          break;
        }
      }
    }
  }
}
