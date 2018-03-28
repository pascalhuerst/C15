#pragma once

#include <proxies/hwui/buttons.h>
#include <cstring>
#include <assert.h>
#include "Selector.h"
#include "TemplateEnums.h"
#include "EventSink.h"

namespace UI {
    template<class T>
    using list = std::initializer_list<T>;

    struct Template {
        const char *id;
        Selector selector;
        list<TemplateElement> elements;
        list<EventSourceMapping> sourceMapping;
        list<EventSinkMapping> sinkMappings;
    };

    /*
     * EventSourceEnum source;
    const char* targetID;
    UI::ComponentValues valueTarget;
    std::function<std::any(std::any)> converter;
     */

    class TemplateRegistry {
    private:
        TemplateRegistry() {
          registerTemplate("unmodulateable-parameter",
                           {UIFocus::Parameters, UIMode::Select, nullptr},
                           {
                                   {
                                               "group", UI::Components::Label,
                                               {0,0,64,16},
                                               {
                                                       {UI::Style::BackgroundColor, (int)UI::StyleValues::Color::C179},
                                                       {UI::Style::ForegroundColor, (int)UI::StyleValues::Color::C43}
                                               }
                                   }
                           },
                           {
                                   {EventSourceEnum::paramterGroupName, "group", UI::ComponentValues::text, nullptr}
                           },
                           {
                                   {EventSinkEnum ::decParam, BUTTON_DEC},
                                   {EventSinkEnum ::incParam, BUTTON_INC}
                           }
          );
        }
    public:
        static TemplateRegistry& get() {
          static TemplateRegistry templateRegister;
          return templateRegister;
        }

        void registerTemplate(const char *id, Selector selector, list<TemplateElement> elements,
                              list<EventSourceMapping> sourceMapping, list<EventSinkMapping> sinkMappings) {
          m_templates.push_back({id, selector, elements, sourceMapping, sinkMappings});
        }


        Template& findTemplate(const char* name) {
          auto it = std::find_if(m_templates.begin(), m_templates.end(), [&name](Template t) {
            return strcmp(t.id, name) == 0;
          });
          assert(it != m_templates.end());
          return *it;
        }

        Template& findTemplate(std::pair<UIMode, UIFocus> modeAndFocus) {
          auto it = std::find_if(m_templates.begin(), m_templates.end(), [&modeAndFocus](Template t) {
            auto& selCB = t.selector.m_detail;
            return t.selector.m_focus == modeAndFocus.second &&
                    t.selector.m_mode == modeAndFocus.first &&
                    (!selCB || selCB());
          });
          assert(it != m_templates.end());
          return *it;
        }

        std::list<Template> m_templates;
    };
}