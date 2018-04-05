#pragma once

#include <playground.h>
#include <proxies/hwui/FrameBuffer.h>

namespace DescriptiveLayouts
{
  using ComponentID = Glib::ustring;

  enum class PrimitiveClasses
  {
    Bar, Border, Text
  };

  enum class PrimitiveProperty
  {
    none, text, range, visibility
  };

  enum class StyleKey
  {
    Color, TextAlign, FontSize, BorderStyle
  };

  enum class ParameterType
  {
    Unipolar = 1 << 0, Bipolar = 1 << 1,
  };

  enum class ButtonEvents
  {
    Down = true, Up = false
  };

  enum class ButtonModifiers
  {
    None = 0, Shift = 1 << 0
  };

  namespace StyleValues
  {
    enum class Font
    {
      Bold, Regular
    };

    enum class Color
    {
      Transparent = FrameBuffer::Transparent,
      C43 = FrameBuffer::C43,
      C77 = FrameBuffer::C77,
      C103 = FrameBuffer::C103,
      C128 = FrameBuffer::C128,
      C179 = FrameBuffer::C179,
      C204 = FrameBuffer::C204,
      C255 = FrameBuffer::C255
    };

    enum class Alignment
    {
      Left, Center, Right
    };

    enum class BorderStyle
    {
      Solid, Rounded, None
    };
  }

  enum class EventSources
  {
    none, parameterName, sliderRange, parameterType, parameterDisplayString, parameterGroupName
  };

  enum class PrimitiveInstances
  {
    none, border, background, centerMark, slider, text
  };

  enum class ControlClasses
  {
    slider, label
  };

  enum class ControlInstances
  {
    groupHeader, slider
  };

  enum class LayoutInstances
  {
      unmodulateableParameterLayout
  };
}
