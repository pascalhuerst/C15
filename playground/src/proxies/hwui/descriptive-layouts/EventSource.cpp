#include "EventSource.h"
#include "LayoutFactory.h"
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

        T m_lastValue { };
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

  class ParameterTypeEventSource : public EventSource<uint8_t>
   {
     public:
      ParameterTypeEventSource()
       {
         Application::get().getPresetManager()->getEditBuffer()->onSelectionChanged(
             sigc::mem_fun(this, &ParameterTypeEventSource::onParameterSelectionChanged));
       }

     private:
       void onParameterSelectionChanged(Parameter *oldParam, Parameter *newParam)
       {
         if(newParam)
         {
           uint8_t mask = 0;

           if(newParam->isBiPolar())
             mask |= (uint8_t)ParameterType::Bipolar;
           else
             mask |= (uint8_t)ParameterType::Unipolar;

           setValue(mask);
         }
       }
   };

  class SliderRangeEventSource : public EventSource<std::pair<tControlPositionValue, tControlPositionValue>>
  {
    public:
      SliderRangeEventSource()
      {
        Application::get().getPresetManager()->getEditBuffer()->onSelectionChanged(
            sigc::mem_fun(this, &SliderRangeEventSource::onParameterSelectionChanged));
      }

    private:
      void onParameterSelectionChanged(Parameter *oldParam, Parameter *newParam)
      {
        m_connection.disconnect();
        if(newParam)
          m_connection = newParam->onParameterChanged(sigc::mem_fun(this, &SliderRangeEventSource::onParameterChanged), true);
      }

      void onParameterChanged(const Parameter* p)
      {
        auto v = p->getControlPositionValue();

        if(p->isBiPolar())
        {
          auto value = (v + 1) / 2;
          auto width = value - 0.5;

          setValue(std::make_pair(0.5, width));
        }
        else
        {
          setValue(std::make_pair(0, v));
        }
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
    m_map[EventSources::ParameterGroupName] = std::make_unique<ParameterGroupNameEventSource>();
    m_map[EventSources::SliderRange] = std::make_unique<SliderRangeEventSource>();
    m_map[EventSources::ParameterType] = std::make_unique<ParameterTypeEventSource>();

    m_map[EventSources::ParameterName];
    m_map[EventSources::ParameterDisplayString];
  }

  sigc::connection EventSourceBroker::connect(EventSources source, std::function<void(std::any)> cb)
  {
    return m_map.at(source)->connect(cb);
  }
}
