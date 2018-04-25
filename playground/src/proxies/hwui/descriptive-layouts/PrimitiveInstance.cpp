#include "PrimitiveInstance.h"
#include "PrimitivesFactory.h"
#include "Styles.h"

namespace DescriptiveLayouts
{
  PrimitiveInstance::PrimitiveInstance(PrimitiveInstances primitiveInstance, PrimitiveClasses type, Rect relativePosition, PrimitiveProperty eventTarget,
                       Converter converter) :
      primitiveInstance(primitiveInstance),
      primitveClass(type),
      relativePosition(relativePosition),
      eventTarget(eventTarget),
      converter(converter)
  {
  }

  Control* PrimitiveInstance::instantiate() const
  {
    return PrimitivesFactory::create(*this);
  }
}
