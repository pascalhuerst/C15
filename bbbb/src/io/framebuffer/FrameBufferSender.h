#pragma once

#include <io/Sender.h>
#include <stddef.h>
#include <linux/fb.h>

struct fb_var_screeninfo;

class FrameBufferSender : public Sender, public sigc::trackable
{
  using super = Sender;

 public:
  FrameBufferSender();
  virtual ~FrameBufferSender();

  void send(tMessage msg) override;

 private:
  bool deferredSend();

  using tPixel = uint8_t;

  tMessage m_currentFrame;
  int m_fd = -1;
  size_t m_buffersize = 0;
  tPixel *m_frontBuffer = nullptr;
  struct fb_var_screeninfo m_varInfo;
};
