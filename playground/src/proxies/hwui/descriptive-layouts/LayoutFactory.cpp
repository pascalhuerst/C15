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
    ControlPrototype slider128x12(ControlClasses::slider,
        Primitive(PrimitiveInstances::border, PrimitiveClasses::Border, Rect(0, 0, 128, 12)),
        Primitive(PrimitiveInstances::background, PrimitiveClasses::Bar, Rect(1, 1, 126, 10)),
        Primitive(PrimitiveInstances::slider, PrimitiveClasses::Bar, Rect(2, 2, 124, 8), PrimitiveProperty::range, nullptr),
        Primitive(PrimitiveInstances::centerMark, PrimitiveClasses::Bar, Rect(63, 2, 2, 8)));

    ControlPrototype label64x16(ControlClasses::label,
        Primitive(PrimitiveInstances::background, PrimitiveClasses::Bar, Rect(0, 0, 64, 16), PrimitiveProperty::visibility, nullptr),
        Primitive(PrimitiveInstances::text, PrimitiveClasses::Text, Rect(0, 0, 64, 16), PrimitiveProperty::text, nullptr));

    registerLayout(LayoutInstances::unmodulateableParameterLayout, Selector(UIFocus::Parameters), Selector(UIMode::Select),
        ControlInstance(ControlInstances::groupHeader, label64x16, Point(0, 0), EventSources::parameterGroupName, PrimitiveInstances::text),
        ControlInstance(ControlInstances::slider, slider128x12, Point(64, 20), EventSources::sliderRange, PrimitiveInstances::slider),
        EventSinkMapping(BUTTON_INC, EventSinks::incParam), EventSinkMapping(BUTTON_DEC, EventSinks::decParam));
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
