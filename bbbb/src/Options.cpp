#include "Options.h"
#include <glibmm/optiongroup.h>

Options::Options(int &argc, char **&argv)
{
  Glib::OptionGroup mainGroup("common", "common options");
  Glib::OptionContext ctx;

  Glib::OptionEntry turnAroundStopWatch;
  turnAroundStopWatch.set_long_name("turn-around-stop-watch");
  turnAroundStopWatch.set_short_name('t');
  turnAroundStopWatch.set_description(
      "sends packet ids with encoder events and stops the time until the id is seen in frame buffer updates");
  mainGroup.add_entry(turnAroundStopWatch, m_bTurnAroundStopWatch);

  ctx.set_main_group(mainGroup);
  ctx.set_help_enabled(true);

  ctx.parse(argc, argv);
}

Options::~Options()
{
}
