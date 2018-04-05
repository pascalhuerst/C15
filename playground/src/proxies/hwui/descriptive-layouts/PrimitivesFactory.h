#pragma once

class Control;

namespace DescriptiveLayouts
{

  struct Primitive;

  struct PrimitivesFactory
  {
      static Control* create(const Primitive& c);
  };

}
