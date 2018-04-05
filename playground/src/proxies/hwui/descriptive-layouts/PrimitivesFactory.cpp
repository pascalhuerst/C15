#include "PrimitivesFactory.h"
#include "TemplateEnums.h"
#include "Primitive.h"
#include "Bar.h"
#include "Border.h"
#include "Text.h"

namespace DescriptiveLayouts
{
  Control* PrimitivesFactory::create(const Primitive& primitive)
  {
    switch(primitive.type)
    {
      case PrimitiveClasses::Bar:
        return new Bar(primitive);
      case PrimitiveClasses::Border:
        return new Border(primitive);
      case PrimitiveClasses::Text:
        return new Text(primitive);
      default:
        throw new std::runtime_error("Primitive Type not found.");
    }
  }
}
