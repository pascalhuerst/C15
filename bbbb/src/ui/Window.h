#pragma once

#ifdef _DEVELOPMENT_PC

#include <gtkmm-3.0/gtkmm.h>
#include <io/network/WebSocketServer.h>
#include "PanelUnit.h"
#include "PlayPanel.h"
#include "Ribbon.h"
#include "LowerRibbon.h"

class Window : public Gtk::Window
{
  public:
    Window();
    virtual ~Window();

  private:
   void onFrameBufferMessageReceived(const WebSocketServer::tMessages &msg);
   void onPanelLEDsMessageReceived(const WebSocketServer::tMessages &msg);
   WebSocketServer::tMessage m_frameBuffer;

   Gtk::VBox m_box;
   Gtk::VBox m_ribbonBox;
   PlayPanel m_playPanel;
   PanelUnit m_editPanel;
   Ribbon m_ribbonUp;
   LowerRibbon m_ribbonDown;
};

#endif
