#include "ControlRegistry.h"

void DescriptiveLayouts::ControlRegistry::registerControl(std::string key, DescriptiveLayouts::ControlClass cp)
{
  m_controlRegistry[key] = std::make_unique<ControlClass>(cp);
}

void DescriptiveLayouts::ControlRegistry::clear()
{
  m_controlRegistry.clear();
}

DescriptiveLayouts::ControlRegistry& DescriptiveLayouts::ControlRegistry::get()
{
  static ControlRegistry cr;
  return cr;
}
