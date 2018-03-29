#pragma once

#include <cstring>
#include <assert.h>
#include <proxies/hwui/DFBLayout.h>
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
    ForegroundColor, BackgroundColor, BorderColor, TextAlign, FontSize, BorderStyle, BorderWidth
  };

  namespace StyleValues
  {
    enum class Font
    {
      Bold, Regular
    };

    enum class Color
    {
      C43, C77, C103, C128, C179, C204, C255, Transparent
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

  struct TemplateElement
  {
      const char* id;
      Components c;
      Rect pos;
      std::initializer_list<std::pair<Style, int>> style;
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

  template<class T>
    using list = std::initializer_list<T>;

  struct Template
  {
      const char *id;
      Selector selector;
      list<TemplateElement> elements;
      list<EventSourceMapping> sourceMapping;
      list<EventSinkMapping> sinkMappings;
  };

  class BoledLayoutFactory
  {
    public:
      static BoledLayoutFactory& get();

      std::shared_ptr<DFBLayout> instantiate(UIFocus focus, UIMode mode);

    private:
      BoledLayoutFactory();

      void registerTemplate(const char* id,
                            Selector selector,
                            list<TemplateElement> elements,
                            list<EventSourceMapping> sourceMapping,
                            list<EventSinkMapping> sinkMappings);

      Template& findTemplate(const char* name);
      Template& findTemplate(UIFocus focus, UIMode mode);

      std::list<Template> m_templates;
  };

}
