#include "Button.h"
#include <gdkmm/rgba.h>

Button::Button(int buttonId) :
    m_buttonId(buttonId)
{
  set_size_request(1, 1);
  set_label(std::to_string(buttonId));
}

Button::~Button()
{
}

void Button::setLed(int idx, bool state)
{
  if(m_buttonId == idx)
  {
    if(m_state != state)
    {
      m_state = state;
      queue_draw();
    }
  }
}

bool Button::on_draw(const ::Cairo::RefPtr<::Cairo::Context>& cr)
{
  Gtk::Button::on_draw(cr);

  if(m_state)
  {
    cr->begin_new_path();
    auto x = get_allocated_width() / 2;
    auto y = get_allocated_height() / 2;
    auto radius = std::min(x, y) / 2;
    cr->arc(x, y, radius, 0, 2 * M_PI);
    cr->set_source_rgb(1, 0, 0);
    cr->set_line_width(0.5);
    cr->close_path();
    cr->fill_preserve();
    cr->set_source_rgb(0.2, 0.2, 0.2);
    cr->stroke();
  }

  return true;
}

void Button::on_pressed()
{

}

void Button::on_released()
{

}

