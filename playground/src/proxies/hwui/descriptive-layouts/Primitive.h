#pragma once

#include <functional>
#include <any>
#include "TemplateEnums.h"
#include <proxies/hwui/controls/Rect.h>

class Control;

namespace DescriptiveLayouts
{

  class Primitive
  {
    public:
      using Converter = std::function<std::any(std::any)>;

      Primitive(PrimitiveInstances primitiveInstance, PrimitiveClasses type, Rect relativePosition, PrimitiveProperty eventTarget = PrimitiveProperty::None,
                Converter converter = nullptr);

      Rect getPosition() const;
      Control* instantiate() const;

      PrimitiveInstances getInstance() const;
      PrimitiveClasses getClass() const;

    private:
      PrimitiveInstances primitiveInstance;
      PrimitiveClasses primitveClass;
      Rect relativePosition;
      PrimitiveProperty eventTarget;
      std::function<std::any(std::any)> converter;

      friend class PrimitivesFactory;
  };

}
