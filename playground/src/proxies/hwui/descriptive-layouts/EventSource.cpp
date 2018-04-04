#include "EventSource.h"
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>

namespace DescriptiveLayouts
{
  template<typename T>
    class EventSource : public EventSourceBase
    {
      protected:
        void setValue(const T &v)
        {
          if(v != m_lastValue)
          {
            m_lastValue = v;
            m_outputSignal.send(m_lastValue);
          }
        }

      private:
        std::any getLastValue() const override
        {
          return m_lastValue;
        }

        T m_lastValue = { };
    };

  class ParameterGroupNameEventSource : public EventSource<Glib::ustring>
  {
    public:
      ParameterGroupNameEventSource()
      {
        Application::get().getPresetManager()->getEditBuffer()->onSelectionChanged(
            sigc::mem_fun(this, &ParameterGroupNameEventSource::onParameterSelectionChanged));
      }

    private:
      void onParameterSelectionChanged(Parameter *oldParam, Parameter *newParam)
      {
        setValue(newParam ? newParam->getParentGroup()->getShortName() : "");
      }
  };

  class ParameterValueEventSource : public EventSource<tControlPositionValue>
  {
    public:
      ParameterValueEventSource()
      {
        Application::get().getPresetManager()->getEditBuffer()->onSelectionChanged(
            sigc::mem_fun(this, &ParameterValueEventSource::onParameterSelectionChanged));
      }

    private:
      void onParameterSelectionChanged(Parameter *oldParam, Parameter *newParam)
      {
        m_connection.disconnect();
        if(newParam)
          m_connection = newParam->onParameterChanged(sigc::mem_fun(this, &ParameterValueEventSource::onParameterChanged), true);
      }

      void onParameterChanged(const Parameter* p)
      {
        setValue(p->getControlPositionValue());
      }

      sigc::connection m_connection;
  };

  EventSourceBroker& EventSourceBroker::get()
  {
    static EventSourceBroker s;
    return s;
  }

  EventSourceBroker::EventSourceBroker()
  {
    m_map[EventSources::parameterGroupName] = std::make_unique<ParameterGroupNameEventSource>();
    m_map[EventSources::parameterValue] = std::make_unique<ParameterValueEventSource>();

    m_map[EventSources::parameterName];
    m_map[EventSources::parameterDisplayString];
  }

  sigc::connection EventSourceBroker::connect(EventSources source, std::function<void(std::any)> cb)
  {
    return m_map.at(source)->connect(cb);
  }
}
