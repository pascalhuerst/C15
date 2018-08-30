#pragma once

#include "playground.h"
#include <proxies/hwui/HWUIEnums.h>
#include <map>

class OLEDProxy;
class Font;

class Oleds
{
 public:
  typedef shared_ptr<Font> tFont;

  virtual ~Oleds();

  static Oleds &get();

  void registerProxy(OLEDProxy *proxy);
  void syncRedraw();
  void setDirty();

  tFont getFont(const Glib::ustring &name, int height);
  void deInit();

 private:
  Oleds();
  Oleds(const Oleds &other);
  Oleds &operator=(const Oleds &);

  bool redraw();

  typedef pair<Glib::ustring, int> tKey;

  list<OLEDProxy *> m_proxies;
  std::map<tKey, tFont> m_fonts;
  sigc::connection m_redrawConnection;
};
