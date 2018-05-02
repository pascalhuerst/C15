#include <io/network/WebSocketReceiver.h>
#include <io/network/WebSocketServer.h>
#include <Application.h>
#include <cstring>

WebSocketReceiver::WebSocketReceiver(Domain d)
{
  if(d == Domain::Lpc)
    Application::get().getWebsocketServer()->onMessageReceived(d, sigc::mem_fun(this, &WebSocketReceiver::onLPCMessageRecieved));
  else
    Application::get().getWebsocketServer()->onMessageReceived(d, sigc::mem_fun(this, &WebSocketReceiver::onMessageReceived));
}

WebSocketReceiver::~WebSocketReceiver()
{
}

void WebSocketReceiver::onMessageReceived(tMessage msg)
{
  onDataReceived(msg);
}

void parseLPCMessage(WebSocketReceiver::tMessage msg)
{

  gsize l;
  auto settingType = msg->get_data(l);

  auto f = (uint16_t*)settingType;


}

void WebSocketReceiver::onLPCMessageRecieved(tMessage msg)
{
  parseLPCMessage(msg);
  onDataReceived(msg);
}

