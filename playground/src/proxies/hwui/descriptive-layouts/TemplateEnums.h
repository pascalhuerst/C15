#pragma once

#include <playground.h>
#include <proxies/hwui/FrameBuffer.h>
#include <tools/EnumTools.h>

namespace DescriptiveLayouts
{
  using ComponentID = Glib::ustring;

  ENUM(PrimitiveProperty, uint8_t, None, Text, Range, Visibility);
  ENUM(StyleKey, uint8_t, BackgroundColor, Color, TextAlign, FontSize, BorderStyle);
  ENUM(ParameterType, uint8_t, Unipolar = 1 << 0, Bipolar = 1 << 1);
  ENUM(ButtonEvents, uint8_t, Down = true, Up = false);
  ENUM(ButtonModifiers, uint8_t, None = 0, Shift = 1 << 0);

  namespace StyleValues
  {
    ENUM(Font, uint8_t, Bold, Regular);
    ENUM(Color, int, Transparent = FrameBuffer::Transparent,
         C43 = FrameBuffer::C43, C77 = FrameBuffer::C77, C103 = FrameBuffer::C103, C128 = FrameBuffer::C128,
         C179 = FrameBuffer::C179, C204 = FrameBuffer::C204, C255 = FrameBuffer::C255
    );
    ENUM(Alignment, uint8_t, Left, Center, Right);
    ENUM(BorderStyle, uint8_t, Solid, Rounded, None);
  }

  ENUM(EventSources, uint8_t, Any, None, ParameterName, SliderRange, ParameterType, ParameterDisplayString, ParameterGroupName);
  ENUM(PrimitiveClasses, uint8_t, Any, Bar, Border, Text);
  ENUM(PrimitiveInstances, uint8_t, Any, None, Border, Background, CenterMark, Slider, Text, Cover);
  ENUM(ControlClasses, uint8_t, Any, Slider, Label, Button, EmptyButton);
  ENUM(ControlInstances, uint8_t, Any, GroupHeader, Slider, ParameterName, ParameterValue, ButtonA, ButtonB, ButtonC, ButtonD);
  ENUM(LayoutClasses, uint8_t, Any, UnmodulateableParameterLayout);
}
