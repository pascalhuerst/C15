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

  sigc::connection EventSourceBroker::connect(EventSources source, std::function<void (std::any)> cb)
  {
    //return m_map.at(source).m_outputSignal.connect(cb);
    return {};
  }
}
