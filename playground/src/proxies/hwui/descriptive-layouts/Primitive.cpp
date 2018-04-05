#include "Primitive.h"
#include "PrimitivesFactory.h"

namespace DescriptiveLayouts
{
  Primitive::Primitive(PrimitiveInstances primitiveInstance, PrimitiveClasses type, Rect relativePosition, PrimitiveProperty eventTarget,
                       Converter converter) :
      primitiveInstance(primitiveInstance),
      type(type),
      relativePosition(relativePosition),
      eventTarget(eventTarget),
      converter(converter)
  {
  }

  Control* Primitive::instantiate() const
  {
    return PrimitivesFactory::create(*this);
  }

  Rect Primitive::getPosition() const
  {
    return relativePosition;
  }
}
