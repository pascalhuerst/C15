#pragma once

#include <giomm/file.h>
#include <tools/Signal.h>

class LayoutFolderMonitor : public sigc::trackable
{
  public:
    static LayoutFolderMonitor &get();
    virtual ~LayoutFolderMonitor();

    void bruteForce();
    sigc::connection onChange(std::function<void ()> cb);

  private:
    LayoutFolderMonitor();

    void onFileChanged(const Glib::RefPtr<Gio::File>&,const Glib::RefPtr<Gio::File>&, Gio::FileMonitorEvent);
    void handleFolder(Glib::RefPtr<Gio::File>& folder);

    Glib::RefPtr<Gio::File> m_rootFolder;
    Glib::RefPtr<Gio::FileMonitor> m_rootMonitor;
    Signal<void> m_onChange;
    std::map<Glib::RefPtr<Gio::File>, Glib::RefPtr<Gio::FileMonitor>> m_monitorMap;
};

