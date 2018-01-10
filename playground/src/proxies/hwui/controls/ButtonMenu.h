#pragma once

#include "ControlWithChildren.h"

class Application;

class ButtonMenu : public ControlWithChildren
{
  private:
    typedef ControlWithChildren super;

  public:
    ButtonMenu (const Rect &rect, size_t numButtonPlaces = 5);
    virtual ~ButtonMenu ();

    using Action = function<void ()>;

    size_t addButton (const Glib::ustring &caption, Action action);
    virtual void selectButton (size_t i);
    size_t getSelectedButton () const;
    void setHighlight(bool isHighlight) override;
    void toggle ();

    void doAction();

  protected:
    void clearActions();

  private:
    void bruteForce ();
    size_t getItemToShowAtPlace(size_t place) const;

    struct Item
    {
        Glib::ustring title;
        Action action;
    };

    vector<Item> m_items;
    size_t m_selected;
    size_t m_numButtonPlaces;
};

