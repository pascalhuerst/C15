#pragma once

#ifdef _DEVELOPMENT_PC

#include <gtkmm-3.0/gtkmm.h>
#include "ButtonPanel.h"

class PanelUnit : public Gtk::HBox
{
  public:
    PanelUnit();
    virtual ~PanelUnit();

    void setLed(int idx, bool state);

  private:
    ButtonPanel* m_panels[4];
};

#endif
