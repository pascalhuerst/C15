#include <proxies/hwui/controls/Button.h>
#include <proxies/hwui/FrameBuffer.h>

Button::Button (const Glib::ustring &caption, int buttonID) :
    super (caption, getButtonPos (buttonID))
{
}

Button::Button (const Glib::ustring &caption, const Rect &rect) :
    super (caption, rect)
{
}

Button::~Button ()
{
}

void Button::blind (bool b)
{
  if (m_blind != b)
  {
    m_blind = b;
    setDirty ();
  }
}

void Button::setFontColor (FrameBuffer &fb) const
{
  if (isHighlight ())
    fb.setColor (FrameBuffer::Colors::C255);
  else
    fb.setColor (FrameBuffer::Colors::C204);
}

bool Button::redraw (FrameBuffer &fb)
{
  auto r = getPosition ();

  if (isHighlight ())
    fb.setColor (FrameBuffer::Colors::C128);
  else
    fb.setColor (FrameBuffer::Colors::C77);

  fb.fillRect (r.getMargined (2, 2));

  if (isHighlight ())
    fb.setColor (FrameBuffer::Colors::C179);
  else
    fb.setColor (FrameBuffer::Colors::C103);

  fb.drawHorizontalLine (r.getLeft () + 1, r.getTop (), r.getWidth () - 2);
  fb.drawVerticalLine (r.getLeft (), r.getTop () + 1, r.getHeight () - 2);
  fb.drawHorizontalLine (r.getLeft () + 1, r.getBottom (), r.getWidth () - 2);
  fb.drawVerticalLine (r.getRight (), r.getY () + 1, r.getHeight () - 2);

  if (!m_blind)
    super::redraw (fb);

  return true;
}

Rect Button::getButtonPos (int n)
{
  switch (n)
  {
    case BUTTON_A:
      return Rect (3, 51, 58, 11);

    case BUTTON_B:
      return Rect (67, 51, 58, 11);

    case BUTTON_C:
      return Rect (131, 51, 58, 11);

    case BUTTON_D:
      return Rect (195, 51, 58, 11);
  }
  return Rect (3, 51, 58, 11);
}
