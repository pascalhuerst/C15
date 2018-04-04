#include "LayoutFactory.h"
#include "GenericLayout.h"
#include <Application.h>
#include <proxies/hwui/HWUI.h>

namespace DescriptiveLayouts
{
  BoledLayoutFactory::BoledLayoutFactory()
  {
    static std::initializer_list<StyleRule> sliderBorderStyle =
    {
     { Style::Color, (int) (StyleValues::Color::C204) },
     { Style::BorderStyle, (int) (StyleValues::BorderStyle::Solid) }
    };

    static std::initializer_list<StyleRule> groupHeaderStyle =
    {
     { Style::Color, (int) (StyleValues::Color::C179) }
    };

    static std::initializer_list<StyleRule> sliderBarStyle =
    {
     { Style::Color, (int) (StyleValues::Color::C204) }
    };

    registerTemplate("unmodulateable-parameter",
        { UIFocus::Parameters, UIMode::Select, nullptr },
        {
          { "group", Components::BasicBuildingBlock, { 0, 0, 64, 16 }, groupHeaderStyle} ,
          { "slider-border", Components::BasicBuildingBlock, { 64, 32, 128, 8 }, sliderBorderStyle },
          { "slider-bar", Components::BasicBuildingBlock, { 66, 34, 0, 4 }, sliderBarStyle}
        },
        {
          { EventSources::parameterGroupName, "group", ComponentValues::text, nullptr },
          { EventSources::parameterValue, "slider-bar", ComponentValues::length, [](std::any in)
            {
              tControlPositionValue v = std::any_cast<tControlPositionValue>(in);
              return v * 124;
            } }
        },
        {
          { EventSinks::decParam, BUTTON_DEC },
          { EventSinks::incParam, BUTTON_INC }
        });
  }

  BoledLayoutFactory& BoledLayoutFactory::get()
  {
    static BoledLayoutFactory factory;
    return factory;
  }

  void BoledLayoutFactory::registerTemplate(const char* id, Selector selector,
                                            std::initializer_list<TemplateElement> elements,
                                            std::initializer_list<EventSourceMapping> sourceMapping,
                                            std::initializer_list<EventSinkMapping> sinkMappings)
  {
    m_templates.push_back( { id, selector, elements, sourceMapping, sinkMappings });
  }

  Template& BoledLayoutFactory::findTemplate(const char* name)
  {
    auto it = std::find_if(m_templates.rbegin(), m_templates.rend(), [&name](const Template &t)
    {
      return strcmp(t.id, name) == 0;
    });
    assert(it != m_templates.rend());
    return *it;
  }

  Template& BoledLayoutFactory::findTemplate(UIFocus focus, UIMode mode)
  {
    auto it = std::find_if(m_templates.rbegin(), m_templates.rend(), [=](const Template &t)
    {
      auto& selCB = t.selector.m_detail;
      return t.selector.m_focus == focus && t.selector.m_mode == mode && (!selCB || selCB());
    });

    assert(it != m_templates.rend());
    return *it;
  }

  std::shared_ptr<DFBLayout> BoledLayoutFactory::instantiate(UIFocus focus, UIMode mode)
  {
    auto temp = findTemplate(focus, mode);
    auto &boled = Application::get().getHWUI()->getPanelUnit().getEditPanel().getBoled();
    auto ret = new GenericLayout({temp}, boled);
    return std::shared_ptr<DFBLayout>(ret);
  }
}
