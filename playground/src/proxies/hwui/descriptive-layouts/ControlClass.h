#pragma once

#include "TemplateEnums.h"
#include "PrimitiveInstance.h"
#include <list>

namespace DescriptiveLayouts
{

  class ControlClass
  {
    public:
      template<typename ...Args>
        ControlClass(ControlClasses controlClass, Args ... args) :
            controlClass(controlClass),
            primitves { args... }
        {
        }

    private:
      ControlClasses controlClass;
      std::list<PrimitiveInstance> primitves;

      friend class GenericControl;
  };

}
