#include "GenericControl.h"
#include "Stylable.h"

namespace DescriptiveLayouts
{

  GenericControl::GenericControl(const ControlInstance &prototype) :
      ControlWithChildren(Rect(prototype.position, Point(0,0))), m_prototype(prototype)
  {
    addPrimitives();
  }

  GenericControl::~GenericControl()
  {
  }

  void GenericControl::addPrimitives()
  {
    auto rect = getPosition();

    int maxX = 0;
    int maxY = 0;

    for(auto &p : m_prototype.control.primitves)
    {
      auto c = addControl(p.instantiate());
      maxX = std::max(maxX, c->getPosition().getRight());
      maxY = std::max(maxY, c->getPosition().getBottom());
    }

    rect.setWidth(maxX);
    rect.setHeight(maxY);
    setPosition(rect);
  }

  void GenericControl::style(LayoutInstances layout)
  {
    for(auto &p : getControls())
    {
      if(auto a = dynamic_pointer_cast<Stylable>(p))
      {
        a->applyStyle({});
      }
    }
  }
}
