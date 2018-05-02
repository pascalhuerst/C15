#pragma once

#ifdef _DEVELOPMENT_PC

#include <gtkmm-3.0/gtkmm.h>

class PlayPanel : public Gtk::VBox
{
  public:
    PlayPanel();
    virtual ~PlayPanel();

  private:
    Gtk::HBox m_upperBox;
    Gtk::VBox m_upperBoxLeft;
    Gtk::VBox m_upperBoxCenter;
    Gtk::VBox m_upperBoxRight;
};

#endif
