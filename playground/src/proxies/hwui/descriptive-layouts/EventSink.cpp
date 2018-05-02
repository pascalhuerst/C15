#include "EventSink.h"
#include "Application.h"
#include "presets/PresetManager.h"
#include "presets/EditBuffer.h"
#include "parameters/Parameter.h"
#include "proxies/hwui/HWUI.h"

namespace DescriptiveLayouts
{
  EventSinkMapping::EventSinkMapping(Buttons button, EventSinks sink, ButtonEvents event, ButtonModifiers modifiers) :
      button(button),
      sink(sink),
      event(event),
      modifiers(modifiers)
  {
  }

  EventSinkBroker& EventSinkBroker::get()
  {
    static EventSinkBroker s;
    return s;
  }

  EventSinkBroker::EventSinkBroker()
  {
    auto eb = Application::get().getPresetManager()->getEditBuffer();
    auto hwui = Application::get().getHWUI();

    registerEvent(EventSinks::IncParam, [eb, hwui]()
    {
      if(auto p = eb->getSelected())
      p->getValue().inc(Initiator::EXPLICIT_HWUI, hwui->getButtonModifiers());
    });

    registerEvent(EventSinks::DecParam, [eb, hwui]()
    {
      if(auto p = eb->getSelected())
      p->getValue().dec(Initiator::EXPLICIT_HWUI, hwui->getButtonModifiers());
    });
  }

  void EventSinkBroker::registerEvent(EventSinks sink, tAction action)
  {
    m_map[sink] = action;
  }

  void EventSinkBroker::fire(EventSinks s)
  {
    m_map.at(s)();
  }
}
