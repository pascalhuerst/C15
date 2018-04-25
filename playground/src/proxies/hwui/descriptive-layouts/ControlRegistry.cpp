#include "ControlRegistry.h"

void DescriptiveLayouts::ControlRegistry::registerControl(std::string key, DescriptiveLayouts::ControlPrototype cp) {
  m_controlRegistry[key] = std::make_unique<ControlPrototype>(cp);
}

DescriptiveLayouts::ControlRegistry& DescriptiveLayouts::ControlRegistry::get() {
  static ControlRegistry cr;
  return cr;
}
