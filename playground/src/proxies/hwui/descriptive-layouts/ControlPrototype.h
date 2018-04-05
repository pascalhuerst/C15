#pragma once

#include "TemplateEnums.h"
#include "Primitive.h"
#include <list>

namespace DescriptiveLayouts
{

  class ControlPrototype
  {
    public:
      template<typename ...Args>
        ControlPrototype(ControlClasses controlClass, Args ... args) :
            controlClass(controlClass),
            primitves { args... }
        {
        }

    private:
      ControlClasses controlClass;
      std::list<Primitive> primitves;

      friend class GenericControl;
  };

}
