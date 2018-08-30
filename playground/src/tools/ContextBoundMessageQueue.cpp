#include "ContextBoundMessageQueue.h"

ContextBoundMessageQueue::ContextBoundMessageQueue(Glib::RefPtr<Glib::MainContext> context)
    : m_context(context)
{
}

void ContextBoundMessageQueue::pushMessage(tMessage&& m)
{
  m_context->signal_timeout().connect_once(m, 0);
}
