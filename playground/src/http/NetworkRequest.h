#pragma once

#include "playground.h"
#include <functional>
#include <libsoup/soup.h>

class OutStream;

class NetworkRequest
{
  public:
    NetworkRequest ();
    virtual ~NetworkRequest ();

    Glib::ustring get (const Glib::ustring &key, Glib::ustring def = "") const;
    void forEach (std::function<void (const Glib::ustring &key, const Glib::ustring &val) > cb);

    virtual Glib::ustring getPath() = 0;

    virtual void pause() = 0;
    virtual void unpause() = 0;
    virtual void okAndComplete() = 0;
    virtual shared_ptr<OutStream> createStream(const Glib::ustring &contentType, bool zip) = 0;
    virtual bool isOracle() const;

  protected:
    void createMapFromQueryString (const Glib::ustring &queryString);

  private:
    typedef std::map<Glib::ustring, Glib::ustring> tKeyValues;

    static void addKeyValuePairToMap (const char *key, const char *value, tKeyValues *map);

    tKeyValues m_keyValues;
};
