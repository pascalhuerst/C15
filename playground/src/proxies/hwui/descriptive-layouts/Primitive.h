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

      Primitive(PrimitiveInstances primitiveInstance, PrimitiveClasses type, Rect relativePosition, PrimitiveProperty eventTarget = PrimitiveProperty::none,
                Converter converter = nullptr);

      Rect getPosition() const;
      Control* instantiate() const;

    private:
      PrimitiveInstances primitiveInstance;
      PrimitiveClasses type;
      Rect relativePosition;
      PrimitiveProperty eventTarget;
      std::function<std::any(std::any)> converter;

      friend class PrimitivesFactory;
  };

}
