#pragma once

#include <cstring>
#include <assert.h>
#include <list>
#include <proxies/hwui/DFBLayout.h>
#include <proxies/hwui/FrameBuffer.h>
#include <proxies/hwui/controls/Rect.h>
#include "EventSink.h"
#include "EventSource.h"
#include "Selector.h"


namespace DescriptiveLayouts
{
  enum class Components
  {
    BasicBuildingBlock
  };

  enum class ComponentValues
  {
    text, length, left
  };

  enum class Style
  {
    Color, TextAlign, FontSize, BorderStyle
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
      Dotted, Solid, Rounded, None
    };
  }

  using StyleRule = std::pair<Style, int>;

  struct TemplateElement
  {
      const char* id;
      Components c;
      Rect pos;
      std::list<StyleRule> style;
  };

  struct EventSinkMapping
  {
      EventSinks target;
      int hwuiTrigger;
  };

  struct EventSourceMapping
  {
      EventSources source;
      const char* targetID;
      ComponentValues valueTarget;
      std::function<std::any(std::any)> converter;
  };

  struct Template
  {
      const char *id;
      Selector selector;
      std::list<TemplateElement> elements;
      std::list<EventSourceMapping> sourceMapping;
      std::list<EventSinkMapping> sinkMappings;
  };

  class BoledLayoutFactory
  {
    public:
      static BoledLayoutFactory& get();

      std::shared_ptr<DFBLayout> instantiate(UIFocus focus, UIMode mode);
      std::shared_ptr<DFBLayout> instantiate(UIFocus focus, UIMode mode, LayoutDetail layoutDetail);

    private:
      BoledLayoutFactory();

      void registerTemplate(const char* id,
                            Selector selector,
                            std::initializer_list<TemplateElement> elements,
                            std::initializer_list<EventSourceMapping> sourceMapping,
                            std::initializer_list<EventSinkMapping> sinkMappings);

      Template& findTemplate(const char* name);
      Template& findTemplate(FocusAndMode focusAndMode);

      std::list<Template> m_templates;
  };

}
