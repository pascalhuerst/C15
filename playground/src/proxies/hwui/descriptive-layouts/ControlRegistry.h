#pragma once
#include <string>
#include "ControlPrototype.h"

namespace DescriptiveLayouts {

    class ControlRegistry {
    protected:
        ControlRegistry() = default;
    public:
        static ControlRegistry& get();
        void registerControl(std::string key, ControlPrototype cp);
        std::unordered_map<std::string, std::unique_ptr<ControlPrototype>> m_controlRegistry;
    };
};