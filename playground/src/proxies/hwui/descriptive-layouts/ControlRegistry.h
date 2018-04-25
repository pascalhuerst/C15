#pragma once
#include <string>
#include "ControlClass.h"

namespace DescriptiveLayouts {

    class ControlRegistry {
    protected:
        ControlRegistry() = default;
    public:
        static ControlRegistry& get();
        void registerControl(std::string key, ControlClass cp);
        std::unordered_map<std::string, std::unique_ptr<ControlClass>> m_controlRegistry;
    };
};