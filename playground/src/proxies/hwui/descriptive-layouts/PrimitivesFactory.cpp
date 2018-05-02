#include "PrimitivesFactory.h"
#include "TemplateEnums.h"
#include "PrimitiveInstance.h"
#include "Bar.h"
#include "Border.h"
#include "Text.h"

namespace DescriptiveLayouts
{
  Control* PrimitivesFactory::create(const PrimitiveInstance& primitive)
  {
    switch(primitive.primitveClass)
    {
      case PrimitiveClasses::Bar:
        return new Bar(primitive);
      case PrimitiveClasses::Border:
        return new Border(primitive);
      case PrimitiveClasses::Text:
        return new Text(primitive);
      default:
        throw new std::runtime_error("PrimitiveInstance Type not found.");
    }
  }
}
