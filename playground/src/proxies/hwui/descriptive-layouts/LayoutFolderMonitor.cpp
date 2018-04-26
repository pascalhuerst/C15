#include "LayoutFolderMonitor.h"
#include "LayoutFactory.h"
#include <Application.h>
#include <Options.h>
#include "LayoutParser.h"
#include "ControlRegistry.h"
#include "ControlParser.h"
#include "Styles.h"
#include "StyleParser.h"

LayoutFolderMonitor& LayoutFolderMonitor::get()
{
  static LayoutFolderMonitor mon;
  return mon;
}

LayoutFolderMonitor::LayoutFolderMonitor()
{
  auto folder = Application::get().getOptions()->getLayoutFolder();
  m_file = Gio::File::create_for_path(folder);
  m_monitor = m_file->monitor(Gio::FILE_MONITOR_WATCH_MOUNTS);
  m_monitor->signal_changed().connect(sigc::mem_fun(this, &LayoutFolderMonitor::onFileChanged));
  bruteForce();
}

LayoutFolderMonitor::~LayoutFolderMonitor()
{
}

void LayoutFolderMonitor::onFileChanged(const Glib::RefPtr<Gio::File>&, const Glib::RefPtr<Gio::File>&, Gio::FileMonitorEvent)
{
  bruteForce();
}

void LayoutFolderMonitor::bruteForce()
{
  DescriptiveLayouts::BoledLayoutFactory::get().clear();
  DescriptiveLayouts::ControlRegistry::get().clear();
  DescriptiveLayouts::StyleSheet::get().clear();

  auto enumerator = m_file->enumerate_children();

  while(auto file = enumerator->next_file())
  {
    auto name = file->get_name();
    auto path = m_file->get_path() + '/' + name;

    if(g_str_has_suffix(name.c_str(), ".json"))
    {
      DescriptiveLayouts::importControls(path);
      DescriptiveLayouts::importLayout(path);
      DescriptiveLayouts::importStyles(path);
    }
  }

  m_onChange.send();
}

sigc::connection LayoutFolderMonitor::onChange(std::function<void ()> cb)
{
  return m_onChange.connect(cb);
}
