#ifdef _DEVELOPMENT_PC

#include <Application.h>
#include <ui/Window.h>

class WebSocketReceiver;

constexpr auto framebufferDimX = 256;
constexpr auto framebufferDimY = 96;

Window::Window()
{
  set_default_size(framebufferDimX, framebufferDimY * 2);
  set_size_request(framebufferDimX, framebufferDimY * 2);

  Application::get().getWebsocketServer()->onMessageReceived(
      Domain::Oled, sigc::mem_fun(this, &Window::onFrameBufferMessageReceived));
  Application::get().getWebsocketServer()->onMessageReceived(Domain::PanelLed,
                                                             sigc::mem_fun(this, &Window::onPanelLEDsMessageReceived));
  m_ribbonUp.set_vexpand(false);
  m_ribbonDown.set_vexpand(false);
  m_ribbonBox.pack_start(m_ribbonUp, false, false);
  m_ribbonBox.pack_end(m_ribbonDown, false, false);
  m_ribbonBox.set_homogeneous(true);
  m_box.pack_start(m_playPanel, true, true);
  m_box.pack_end(m_editPanel, false, false);
  m_box.pack_start(m_ribbonBox, false, false);

  add(m_box);
  show_all_children(true);
}

Window::~Window()
{
}

void Window::onFrameBufferMessageReceived(const WebSocketServer::tMessages &msgs)
{
  if(!msgs.empty())
    m_playPanel.setFrameBuffer(msgs.back());
}

void Window::onPanelLEDsMessageReceived(const WebSocketServer::tMessages &msgs)
{
  for(auto &msg : msgs)
  {
    gsize len = 0;
    const int8_t *data = reinterpret_cast<const int8_t *>(msg->get_data(len));
    auto idx = data[0] & 0x7F;
    auto val = data[0] >> 7;
    m_editPanel.setLed(idx, val);
  }
}

#endif
