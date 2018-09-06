#pragma once

#include "playground.h"
#include <libsoup/soup.h>
#include <functional>
#include <memory>
#include <list>
#include <tools/Expiration.h>
#include <thread>
#include <atomic>
#include <tools/ContextBoundMessageQueue.h>

class WebSocketSession
{
 public:
  enum class Domain : uint8_t
  {
    Lpc = 1,
    Buttons = 2,
    Oled = 3,
    Rotary = 4,
    PanelLed = 5,
    RibbonLed = 6
  };

  WebSocketSession();
  virtual ~WebSocketSession();

  using tMessage = Glib::RefPtr<Glib::Bytes>;
  using tMessages = std::list<tMessage>;

  void openGate();

  void sendMessage(Domain d, tMessage msg);
  sigc::connection onMessageReceived(Domain d, const sigc::slot<void, const tMessages &> &cb);
  sigc::connection onConnectionEstablished(const sigc::slot<void> &cb);

 private:
  void connect();
  void connectWebSocket(SoupWebsocketConnection *connection);
  static void onWebSocketConnected(SoupSession *session, GAsyncResult *res, WebSocketSession *pThis);
  static void receiveMessage(SoupWebsocketConnection *self, gint type, GBytes *message, WebSocketSession *pThis);
  void sendMessage(tMessage msg);
  void reconnect();
  void backgroundThread();

  using tSessionPtr = std::unique_ptr<SoupSession, decltype(*g_object_unref)>;
  using tWebSocketPtr = std::unique_ptr<SoupWebsocketConnection, decltype(*g_object_unref)>;
  using tMessagePtr = std::unique_ptr<SoupMessage, decltype(*g_object_unref)>;
  using tSignal = sigc::signal<void, const tMessages &>;
  using tConnectionEstablishedSignal = sigc::signal<void>;

  tSessionPtr m_soupSession;
  tMessagePtr m_message;
  tWebSocketPtr m_connection;
  std::map<Domain, tSignal> m_onMessageReceived;

  std::map<Domain, tMessages> m_incomingMessages;
  std::map<Domain, bool> m_incomingMessagesScheduled;

  tConnectionEstablishedSignal m_onConnectionEstablished;

  std::unique_ptr<ContextBoundMessageQueue> m_backgroundContextQueue;
  std::unique_ptr<ContextBoundMessageQueue> m_defaultContextQueue;
  Glib::RefPtr<Glib::MainLoop> m_messageLoop;
  std::thread m_contextThread;
  bool m_receiveGate = false;
};
