#pragma once

#include <io/files/FileIOReceiver.h>

class MessageParser;

class LPCReceiver : public FileIOReceiver
{
    using super = FileIOReceiver;

  public:
    LPCReceiver();
    virtual ~LPCReceiver();

  protected:
   void onDataReceived(const tMessages &msgs) override;

  private:
    std::unique_ptr<MessageParser> m_parser;
};

