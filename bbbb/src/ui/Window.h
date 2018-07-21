#pragma once

#ifdef _DEVELOPMENT_PC

#include <gtkmm-3.0/gtkmm.h>
#include <io/network/WebSocketServer.h>
#include "Boled.h"

class Window : public Gtk::Window
{
  public:
    Window();
    virtual ~Window();

  private:
    void onFrameBufferMessageReceived(WebSocketServer::tMessage msg);
    WebSocketServer::tMessage m_frameBuffer;

    Gtk::VBox m_box;
    Boled m_boled;
};

#endif
