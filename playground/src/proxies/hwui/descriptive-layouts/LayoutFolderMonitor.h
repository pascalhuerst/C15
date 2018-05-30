#pragma once

#include <giomm/file.h>
#include <tools/Signal.h>
#include <tools/RecursiveDirectoryMonitor.h>

class LayoutFolderMonitor : public sigc::trackable
{
  public:
    static LayoutFolderMonitor &get();

    void bruteForce();
    sigc::connection onChange(std::function<void ()> cb);
  private:
    LayoutFolderMonitor();

    void onFileChanged(const Glib::RefPtr<Gio::File>&,const Glib::RefPtr<Gio::File>&, Gio::FileMonitorEvent);
    using tCallBack = std::function<void(const Glib::RefPtr<Gio::File>&,const Glib::RefPtr<Gio::File>&, Gio::FileMonitorEvent)>;
    tCallBack m_myCallback = [&](const Glib::RefPtr<Gio::File>&,const Glib::RefPtr<Gio::File>&, Gio::FileMonitorEvent){
      bruteForce();
    };
    Signal<void> m_onChange;
    FileTools::RecursiveDirectoryMonitor<decltype(m_myCallback)> m_recMonitor;
    Glib::RefPtr<Gio::File> m_rootFolder;
};

