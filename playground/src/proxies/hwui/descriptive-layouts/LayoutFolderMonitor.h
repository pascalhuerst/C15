#pragma once

#include <giomm/file.h>


class LayoutFolderMonitor
{
  public:
    static LayoutFolderMonitor &get();
    virtual ~LayoutFolderMonitor();

  private:
    LayoutFolderMonitor();

    void onFileChanged(const Glib::RefPtr<Gio::File>&,const Glib::RefPtr<Gio::File>&, Gio::FileMonitorEvent);
    void bruteForce();

    Glib::RefPtr<Gio::File> m_file;
    Glib::RefPtr<Gio::FileMonitor> m_monitor;
};

