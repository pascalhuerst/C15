#include <io/network/WebSocketReceiver.h>
#include <io/network/WebSocketServer.h>
#include <Application.h>
#include <cstring>

WebSocketReceiver::WebSocketReceiver(Domain d)
{
  Application::get().getWebsocketServer()->onMessageReceived(d,
                                                             sigc::mem_fun(this, &WebSocketReceiver::onDataReceived));
}

WebSocketReceiver::~WebSocketReceiver()
{
}
