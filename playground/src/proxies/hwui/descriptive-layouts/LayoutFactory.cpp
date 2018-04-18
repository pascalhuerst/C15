#include "LayoutFactory.h"
#include "PrimitivesFactory.h"
#include "GenericLayout.h"
#include <Application.h>
#include <proxies/hwui/HWUI.h>
#include "ControlPrototype.h"
#include "ControlInstance.h"
#include "Selector.h"
#include "TemplateEnums.h"
#include "EventSink.h"

namespace DescriptiveLayouts
{

  BoledLayoutFactory::BoledLayoutFactory()
  {
    ControlPrototype slider128x12(ControlClasses::Slider,
        Primitive(PrimitiveInstances::Border, PrimitiveClasses::Border, Rect(0, 0, 128, 12)),
        Primitive(PrimitiveInstances::Background, PrimitiveClasses::Bar, Rect(1, 1, 126, 10)),
        Primitive(PrimitiveInstances::Slider, PrimitiveClasses::Bar, Rect(2, 2, 124, 8), PrimitiveProperty::Range, nullptr),
        Primitive(PrimitiveInstances::CenterMark, PrimitiveClasses::Bar, Rect(63, 2, 2, 8))
        );

    ControlPrototype label64x16(ControlClasses::Label,
        Primitive(PrimitiveInstances::Background, PrimitiveClasses::Bar, Rect(0, 0, 64, 16), PrimitiveProperty::Visibility, nullptr),
        Primitive(PrimitiveInstances::Text, PrimitiveClasses::Text, Rect(0, 0, 64, 16), PrimitiveProperty::Text, nullptr));

    registerLayout(LayoutInstances::UnmodulateableParameterLayout, Selector(UIFocus::Parameters), Selector(UIMode::Select),
        ControlInstance(ControlInstances::GroupHeader, label64x16, Point(0, 0), EventSources::ParameterGroupName, PrimitiveInstances::Text),
        ControlInstance(ControlInstances::Slider, slider128x12, Point(64, 20), EventSources::SliderRange, PrimitiveInstances::Slider),
        EventSinkMapping(BUTTON_INC, EventSinks::IncParam), EventSinkMapping(BUTTON_DEC, EventSinks::DecParam));
  }

  BoledLayoutFactory& BoledLayoutFactory::get()
  {
    static BoledLayoutFactory factory;
    return factory;
  }

  const DescriptiveLayouts::LayoutPrototype& BoledLayoutFactory::find(FocusAndMode fam) const
  {
    auto it = std::find_if(m_layouts.begin(), m_layouts.end(), [=](const LayoutPrototype& e)
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
