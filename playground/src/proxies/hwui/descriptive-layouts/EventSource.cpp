#include "EventSource.h"

namespace DescriptiveLayouts
{
  EventSourceBroker& EventSourceBroker::get()
  {
    static EventSourceBroker s;
    return s;
  }

  EventSourceBroker::EventSourceBroker()
  {
    //TODO fill me pls m_map
  }
}
