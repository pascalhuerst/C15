#pragma once

#ifdef _DEVELOPMENT_PC

#include <gtkmm-3.0/gtkmm.h>
#include "ButtonPanel.h"

class EditPanel : public Gtk::HBox
{
  public:
    EditPanel();
    virtual ~EditPanel();

    void setLed(int idx, bool state);

  private:
    std::array<ButtonPanel, 4> m_panels;
};

#endif
