#include <Application.h>
#include <io/TurnAroundStopWatch.h>
#include <Options.h>
#include <bbbb.h>
#include <fcntl.h>
#include <io/framebuffer/FrameBufferSender.h>
#include <Options.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <string.h>

FrameBufferSender::FrameBufferSender()
{
  Glib::MainContext::get_default()->signal_timeout().connect(sigc::mem_fun(this, &FrameBufferSender::deferredSend), 10);

#ifndef _DEVELOPMENT_PC
  m_fd = open("/dev/fb0", O_RDWR);

  if(m_fd < 0)
  {
    TRACE("Could not open framebuffer device file");
    return;
  }

  if(ioctl(m_fd, FBIOGET_VSCREENINFO, &m_varInfo) == -1)
  {
    TRACE("Could not get fb_var_screeninfo!");
    return;
  }

  auto bytesPerPixel = m_varInfo.bits_per_pixel / 8;
  m_buffersize = m_varInfo.xres * m_varInfo.yres * bytesPerPixel;
  m_frontBuffer = reinterpret_cast<tPixel *>(mmap(0, m_buffersize, PROT_READ | PROT_WRITE, MAP_SHARED, m_fd, 0));

  if(m_frontBuffer == MAP_FAILED)
  {
    TRACE("Could not memory map buffer");
    return;
  }
#endif
}

FrameBufferSender::~FrameBufferSender()
{
}

void FrameBufferSender::send(tMessage msg)
{
  m_currentFrame = msg;
}

bool FrameBufferSender::deferredSend()
{
#ifndef _DEVELOPMENT_PC
  if(auto f = std::move(m_currentFrame))
  {
    if(Application::get().getOptions()->isTurnAroundStopWatchEnabled())
    {
      gsize l = 0;
      uint32_t *raw = (uint32_t *) f->get_data(l);
      uint32_t numPacketIDs = raw[0];
      TurnAroundStopWatch::stop(&raw[1], numPacketIDs);

      auto frameBufferBytes = &raw[numPacketIDs + 1];
      auto numFrameBufferBytes = l - 4 * (numPacketIDs + 1);
      memcpy(m_frontBuffer, frameBufferBytes, numFrameBufferBytes);
      msync(m_frontBuffer, numFrameBufferBytes, MS_ASYNC);
    }
    else
    {
      gsize numBytes = 0;
      auto bytes = reinterpret_cast<const int8_t *>(f->get_data(numBytes));
      memcpy(m_frontBuffer, bytes, numBytes);
      msync(m_frontBuffer, numBytes, MS_ASYNC);
    }
  }
#endif

  return true;
}
