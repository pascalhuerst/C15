#include "EventSink.h"

namespace DescriptiveLayouts
{
  EventSinkBroker& EventSinkBroker::get()
  {
    static EventSinkBroker s;
    return s;
  }

  EventSinkBroker::EventSinkBroker()
  {
    //TODO fill me pls m_map
  }
}
