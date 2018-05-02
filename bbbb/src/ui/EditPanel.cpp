#include "EditPanel.h"

#ifdef _DEVELOPMENT_PC

EditPanel::EditPanel() :
    m_panels { 0, 24, 48, 72 }
{
  for(auto &p : m_panels)
    add(p);
}

EditPanel::~EditPanel()
{
}

void EditPanel::setLed(int idx, bool state)
{
  for(auto &p : m_panels)
    p.setLed(idx, state);
}

#endif
