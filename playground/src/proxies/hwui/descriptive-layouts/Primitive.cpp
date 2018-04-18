#include "Primitive.h"
#include "PrimitivesFactory.h"
#include "Styles.h"

namespace DescriptiveLayouts
{
  Primitive::Primitive(PrimitiveInstances primitiveInstance, PrimitiveClasses type, Rect relativePosition, PrimitiveProperty eventTarget,
                       Converter converter) :
      primitiveInstance(primitiveInstance),
      primitveClass(type),
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

  PrimitiveInstances Primitive::getInstance() const
  {
    return primitiveInstance;
  }

  PrimitiveClasses Primitive::getClass() const
  {
    return primitveClass;
  }
}
