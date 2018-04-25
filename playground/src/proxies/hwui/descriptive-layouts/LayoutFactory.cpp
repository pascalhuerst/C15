#include "LayoutFactory.h"
#include "PrimitivesFactory.h"
#include "GenericLayout.h"
#include <Application.h>
#include <proxies/hwui/HWUI.h>
#include "ControlClass.h"
#include "ControlInstance.h"
#include "Selector.h"
#include "TemplateEnums.h"
#include "EventSink.h"
#include "Styles.h"

namespace DescriptiveLayouts
{

  BoledLayoutFactory::BoledLayoutFactory()
  {
    auto &styles = StyleSheet::get();

    styles.registerStyle( { PrimitiveClasses::Bar, PrimitiveInstances::Background },
        { { StyleKey::Color, (int) StyleValues::Color::C43 } });

    styles.registerStyle( { ControlClasses::Button, PrimitiveInstances::Background },
        { { StyleKey::Color, (int) StyleValues::Color::C179 } });

    styles.registerStyle( { ControlClasses::Button, PrimitiveInstances::Border }, { { StyleKey::Color, (int) StyleValues::Color::C204 }, {
        StyleKey::BorderStyle, (int) StyleValues::BorderStyle::Solid } });

    styles.registerStyle( { ControlClasses::Button, PrimitiveClasses::Bar, PrimitiveInstances::Cover },
        { { StyleKey::Color, (int) StyleValues::Color::C43 } });

    styles.registerStyle( { PrimitiveClasses::Bar }, { { StyleKey::Color, (int) StyleValues::Color::C255 } });

    styles.registerStyle( { PrimitiveClasses::Border }, { { StyleKey::Color, (int) StyleValues::Color::C255 }, {
        StyleKey::BorderStyle, (int) StyleValues::BorderStyle::Solid } });

    styles.registerStyle( { PrimitiveClasses::Text }, { { StyleKey::TextAlign, (int) StyleValues::Alignment::Center }, { StyleKey::FontSize,
                                                                                                                         9 },
                                                        { StyleKey::Color, (int) FrameBuffer::C255 } });

    styles.registerStyle( { ControlInstances::GroupHeader, PrimitiveInstances::Background }, { });

    styles.registerStyle( { ControlInstances::GroupHeader, PrimitiveInstances::Text },
        { { StyleKey::Color, (int) StyleValues::Color::C43 }, { StyleKey::BackgroundColor, (int) StyleValues::Color::C255 } });

    ControlClass slider128x10(ControlClasses::Slider,
        PrimitiveInstance(PrimitiveInstances::Border, PrimitiveClasses::Border, Rect(0, 0, 128, 10)),
        PrimitiveInstance(PrimitiveInstances::Background, PrimitiveClasses::Bar, Rect(1, 1, 126, 8)),
        PrimitiveInstance(PrimitiveInstances::Slider, PrimitiveClasses::Bar, Rect(2, 2, 124, 6), PrimitiveProperty::Range, nullptr),
        PrimitiveInstance(PrimitiveInstances::CenterMark, PrimitiveClasses::Bar, Rect(63, 2, 2, 6)));

    ControlClass headerLabel(ControlClasses::Label,
        PrimitiveInstance(PrimitiveInstances::Text, PrimitiveClasses::Text, Rect(0, 0, 64, 16), PrimitiveProperty::Text, nullptr));

    ControlClass label128x14(ControlClasses::Label,
        PrimitiveInstance(PrimitiveInstances::Text, PrimitiveClasses::Text, Rect(0, 0, 128, 14), PrimitiveProperty::Text, nullptr));

    ControlClass emptyButton(ControlClasses::Button,
        PrimitiveInstance(PrimitiveInstances::Background, PrimitiveClasses::Bar, Rect(3, 2, 56, 9)),
        PrimitiveInstance(PrimitiveInstances::Border, PrimitiveClasses::Border, Rect(2, 1, 58, 11)),
        PrimitiveInstance(PrimitiveInstances::Cover, PrimitiveClasses::Bar, Rect(2, 1, 1, 1)),
        PrimitiveInstance(PrimitiveInstances::Cover, PrimitiveClasses::Bar, Rect(59, 1, 1, 1)),
        PrimitiveInstance(PrimitiveInstances::Cover, PrimitiveClasses::Bar, Rect(2, 11, 1, 1)),
        PrimitiveInstance(PrimitiveInstances::Cover, PrimitiveClasses::Bar, Rect(59, 11, 1, 1)));

    registerLayout(LayoutClasses::UnmodulateableParameterLayout, Selector(UIFocus::Parameters), Selector(UIMode::Select),
        ControlInstance(ControlInstances::GroupHeader, headerLabel, Point(0, 0), EventSources::ParameterGroupName,
            PrimitiveInstances::Text),
        ControlInstance(ControlInstances::ParameterName, label128x14, Point(64, 13), EventSources::ParameterName, PrimitiveInstances::Text),
        ControlInstance(ControlInstances::Slider, slider128x10, Point(64, 26), EventSources::SliderRange, PrimitiveInstances::Slider),
        ControlInstance(ControlInstances::ParameterName, label128x14, Point(64, 39), EventSources::ParameterDisplayString,
            PrimitiveInstances::Text),
        ControlInstance(ControlInstances::ButtonA, emptyButton, Point(0, 52)),
        ControlInstance(ControlInstances::ButtonB, emptyButton, Point(64, 52)),
        ControlInstance(ControlInstances::ButtonC, emptyButton, Point(128, 52)),
        ControlInstance(ControlInstances::ButtonD, emptyButton, Point(192, 52)), EventSinkMapping(BUTTON_INC, EventSinks::IncParam),
        EventSinkMapping(BUTTON_DEC, EventSinks::DecParam), EventSinkMapping(ROTARY_PLUS, EventSinks::IncParam),
        EventSinkMapping(ROTARY_MINUS, EventSinks::DecParam));
  }

  BoledLayoutFactory& BoledLayoutFactory::get()
  {
    static BoledLayoutFactory factory;
    return factory;
  }

  const DescriptiveLayouts::LayoutClass& BoledLayoutFactory::find(FocusAndMode fam) const
  {
    auto it = std::find_if(m_layouts.begin(), m_layouts.end(), [=](const LayoutClass& e)
    {
      return e.matches(fam);
    });

    if(it == m_layouts.end())
    {
      throw std::runtime_error("No matching layout found.");
    }

    return *it;
  }

  std::shared_ptr<DFBLayout> BoledLayoutFactory::instantiate(FocusAndMode fam)
  {
    return std::shared_ptr<DFBLayout>(find(fam).instantiate());
  }

  EventSinkMapping::EventSinkMapping(int button, EventSinks sink, ButtonEvents event, ButtonModifiers modifiers) :
      button(button),
      sink(sink),
      event(event),
      modifiers(modifiers)
  {
  }

}
