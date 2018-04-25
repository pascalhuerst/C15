#pragma once

#include <playground.h>
#include <proxies/hwui/FrameBuffer.h>

namespace DescriptiveLayouts
{
  using ComponentID = Glib::ustring;

  enum class PrimitiveProperty
  {
    None, Text, Range, Visibility
  };

  enum class StyleKey
  {
    BackgroundColor, Color, TextAlign, FontSize, BorderStyle
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
    Any, None, ParameterName, SliderRange, ParameterType, ParameterDisplayString, ParameterGroupName
  };

  enum class PrimitiveClasses
  {
      Any, Bar, Border, Text
  };

  enum class PrimitiveInstances
  {
    Any, None, Border, Background, CenterMark, Slider, Text, Cover
  };

  enum class ControlClasses
  {
    Any, Slider, Label, Button
  };

  inline std::list<std::string> ControlClassesStrings{"Any", "Slider", "Label", "Button"};

  inline ControlClasses toControlClasses(std::string key) {
    auto it = std::find(ControlClassesStrings.begin(), ControlClassesStrings.end(), key);
    auto ret = std::distance(ControlClassesStrings.begin(), it);
    return (ControlClasses)ret;
  }

  enum class ControlInstances
  {
    Any, GroupHeader, Slider, ParameterName, ParameterValue, ButtonA, ButtonB, ButtonC, ButtonD
  };

  enum class LayoutClasses
  {
    Any, UnmodulateableParameterLayout
  };
}
