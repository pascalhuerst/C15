#include <device-settings/DebugLevel.h>
#include <fcntl.h>
#include <proxies/hwui/controls/Point.h>
#include <proxies/hwui/FrameBuffer.h>
#include <proxies/hwui/HWUI.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <string.h>
#include <iostream>
#include <io/network/WebSocketSession.h>
#include <Application.h>
#include <tools/TimeTools.h>

FrameBuffer::StackScopeGuard::StackScopeGuard(FrameBuffer *fb)
    : m_fb(fb)
{
}

FrameBuffer::StackScopeGuard::StackScopeGuard(StackScopeGuard &&other)
    : m_fb(nullptr)
{
  swap(m_fb, other.m_fb);
}

FrameBuffer::StackScopeGuard::~StackScopeGuard()
{
}

FrameBuffer::Clip::Clip(FrameBuffer *fb, const Rect &rect)
    : StackScopeGuard(fb)
{
  auto intersection = rect.getIntersection(m_fb->m_clips.top());
  fb->m_clips.push(intersection);
}

FrameBuffer::Clip::Clip(Clip &&other)
    : StackScopeGuard(std::move(other))
{
}

FrameBuffer::Clip::~Clip()
{
  if(m_fb)
    m_fb->m_clips.pop();
}

bool FrameBuffer::Clip::isEmpty() const
{
  return m_fb->m_clips.top().isEmpty();
}

FrameBuffer::Offset::Offset(FrameBuffer *fb, const Point &offset)
    : StackScopeGuard(fb)
{
  m_fb->m_offsets.push(m_fb->m_offsets.top() + offset);
}

FrameBuffer::Offset::Offset(Offset &&other)
    : StackScopeGuard(std::move(other))
{
}

FrameBuffer::Offset::~Offset()
{
  if(m_fb)
    m_fb->m_offsets.pop();
}

FrameBuffer::FrameBuffer()
{
  initStacks();
  openAndMap();
  clear();

  Application::get().getWebSocketSession()->onConnectionEstablished(sigc::mem_fun(this, &FrameBuffer::swapBuffers));
}

void FrameBuffer::initStacks()
{
  m_clips.push(Rect(0, 0, 256, 64 + 32));
  m_offsets.push(Point(0, 0));
}

void FrameBuffer::openAndMap()
{
  auto bytesPerPixel = 1;
  auto buffersize = 256 * 96 * 1;
  m_backBuffer.resize(buffersize);
}

FrameBuffer::~FrameBuffer()
{
  unmapAndClose();
}

void FrameBuffer::setPixel(tCoordinate x, tCoordinate y)
{
  const auto &offset = m_offsets.top();
  setOffsetPixel(x + offset.getX(), y + offset.getY());
}

inline void FrameBuffer::setOffsetPixel(tCoordinate x, tCoordinate y)
{
  if(m_clips.top().contains(x, y))
    setRawPixel(x, y);
}

inline void FrameBuffer::setRawPixel(tCoordinate x, tCoordinate y)
{
  const long index = getIndex(x, y);
  m_backBuffer[index] = m_currentColor;
}

inline long FrameBuffer::getIndex(tCoordinate x, tCoordinate y) const
{
  return (x + y * 256);
}

void FrameBuffer::unmapAndClose()
{
}

void FrameBuffer::clear()
{
  std::fill(m_backBuffer.begin(), m_backBuffer.end(), 0);
}

void FrameBuffer::setColor(const Colors &c)
{
  m_currentColor = c;
}

void FrameBuffer::fiddleColor(tPixel p)
{
  m_currentColor = (Colors)(p);
}

FrameBuffer::Colors FrameBuffer::getColor() const
{
  return m_currentColor;
}

void FrameBuffer::fillRect(tCoordinate left, tCoordinate top, tCoordinate width, tCoordinate height)
{
  fillRect(Rect(left, top, width, height));
}

void FrameBuffer::fillRect(const Rect &rect)
{
  Rect fill = rect.getMovedBy(m_offsets.top()).getIntersection(m_clips.top());

  if(!fill.isEmpty())
  {
    auto left = fill.getLeft();
    auto width = fill.getWidth();
    auto bottom = fill.getBottom();

    for(auto y = fill.getTop(); y <= bottom; y++)
      drawRawHorizontalLine(left, y, width);
  }
}

inline void FrameBuffer::drawRawHorizontalLine(tCoordinate x, tCoordinate y, tCoordinate length)
{
  auto fromIdx = getIndex(x, y);
  auto data = m_backBuffer.data() + fromIdx;

  for(long i = 0; i < length; i++)
    data[i] = m_currentColor;
}

void FrameBuffer::drawRect(tCoordinate x, tCoordinate y, tCoordinate width, tCoordinate height)
{
  drawRect(Rect(x, y, width, height));
}

void FrameBuffer::drawRect(const Rect &rect)
{
  if(!rect.isEmpty())
  {
    drawHorizontalLine(rect.getLeft(), rect.getTop(), rect.getWidth());
    drawHorizontalLine(rect.getLeft(), rect.getBottom(), rect.getWidth());
    drawVerticalLine(rect.getLeft(), rect.getTop(), rect.getHeight());
    drawVerticalLine(rect.getRight(), rect.getTop(), rect.getHeight());
  }
}

void FrameBuffer::drawHorizontalLine(tCoordinate x, tCoordinate y, tCoordinate length)
{
  auto &clip = m_clips.top();
  auto &offset = m_offsets.top();

  x += offset.getX();
  y += offset.getY();

  if(y >= clip.getTop() && y <= clip.getBottom() && length > 0)
  {
    auto left = std::max(x, clip.getLeft());
    auto right = std::min(x + length, clip.getLeft() + clip.getWidth());
    drawRawHorizontalLine(left, y, right - left);
  }
}

void FrameBuffer::drawVerticalLine(tCoordinate x, tCoordinate y, tCoordinate length)
{
  auto &clip = m_clips.top();
  auto &offset = m_offsets.top();

  x += offset.getX();
  y += offset.getY();

  if(x >= clip.getLeft() && x <= clip.getRight() && length > 0)
  {
    auto top = std::max(y, clip.getTop());
    auto bottom = std::min(y + length, clip.getTop() + clip.getHeight());

    for(auto i = top; i < bottom; i++)
      setRawPixel(x, i);
  }
}

void FrameBuffer::swapBuffers()
{
  auto &knob = Application::get().getHWUI()->getPanelUnit().getEditPanel().getKnob();

  decltype(knob.receivedMessageIDs) cp;
  std::swap(cp, knob.receivedMessageIDs);
  uint32_t numIDs = cp.size();

  auto bufSize = m_backBuffer.size() + (1 + cp.size()) * 4;
  uint8_t buf[bufSize];
  memcpy(&buf[0], &numIDs, 4);

  for(int i = 0; i < numIDs; i++)
  {
    std::cerr << "playground rendered encoder event " << cp[i] << " at " << TimeTools::getPerformanceTimeStamp()
              << std::endl;
    memcpy(&buf[4 * (1 + i)], &cp[i], 4);
  }

  memcpy(&buf[+4 * (1 + numIDs)], m_backBuffer.data(), m_backBuffer.size());
  auto bytes = Glib::Bytes::create(buf, bufSize);
  Application::get().getWebSocketSession()->sendMessage(WebSocketSession::Domain::Oled, bytes);
}

FrameBuffer::Clip FrameBuffer::clip(const Rect &rect)
{
  return Clip(this, rect);
}

FrameBuffer::Clip FrameBuffer::clipRespectingOffset(const Rect &rect)
{
  return clip(rect.getMovedBy(m_offsets.top()));
}

FrameBuffer::Offset FrameBuffer::offset(const Point &offset)
{
  return Offset(this, offset);
}
