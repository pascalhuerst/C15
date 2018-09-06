#include <io/files/LPCReceiver.h>
#include "Application.h"
#include "Options.h"
#include "MessageParser.h"

LPCReceiver::LPCReceiver()
    : super("/dev/lpc_bb_driver", MessageParser::getNumInitialBytesNeeded())
    , m_parser(std::make_unique<MessageParser>())
{
}

LPCReceiver::~LPCReceiver()
{
}

void LPCReceiver::onDataReceived(const tMessages &msgs)
{
  for(auto &msg : msgs)
  {
    gsize numBytes = 0;
    auto data = reinterpret_cast<const uint8_t *>(msg->get_data(numBytes));

    if(auto numBytesMissing = m_parser->parse(data, numBytes))
    {
      setBlockSize(numBytesMissing);
    }
    else
    {
      g_assert(m_parser->isFinished());
      super::onDataReceived({ m_parser->getMessage() });
      m_parser = std::make_unique<MessageParser>();
      setBlockSize(MessageParser::getNumInitialBytesNeeded());
    }
  }
}
