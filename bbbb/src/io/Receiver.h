#pragma once

#include "bbbb.h"

class Receiver : public sigc::trackable
{
  public:
    using tMessage = Glib::RefPtr<Glib::Bytes>;
    using tMessages = std::list<tMessage>;
    using Callback = std::function<void(const tMessages &)>;

    Receiver();
    virtual ~Receiver();

    void setCallback(Callback cb);

  protected:
   virtual void onDataReceived(const tMessages &msgs);

  private:
    Callback m_callback;
};

