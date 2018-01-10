#include <proxies/hwui/panel-unit/boled/setup/AboutText.h>

class Rect;

AboutText::AboutText () :
    super ("")
{
  setText("Development:\nStephan Schmitt, Daniel Tzschentke, Simone Fabbri, "
          "Henry Högelow, Nemanja Nikodijevic, Matthias Seeber, Pascal Hürst, "
          "Tino Meinert, Gabriel Mulzer, Mike Daliot, Justus Scheil, Ulrich Wappler, Anton Schmied, Brian Smith", FrameBuffer::Colors::C255);
}

void AboutText::setPosition (const Rect &rect)
{
  super::setPosition (rect);
}

const Rect &AboutText::getPosition () const
{
  return super::getPosition ();
}

void AboutText::setDirty ()
{
  super::setDirty ();
  notifyDirty (true);
}
