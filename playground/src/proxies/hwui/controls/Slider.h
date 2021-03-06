#pragma once

#include "Control.h"
#include "parameters/Parameter.h"

class Parameter;

class Slider : public Control
{
  private:
    typedef Control super;

  public:
    Slider (Parameter * param, const Rect &rect);
    Slider (const Rect &rect);
    virtual ~Slider ();

    void setParameter (Parameter * param);
    Parameter * getParameter () const;

    void setValue (tDisplayValue v, bool bipolar);
    tDisplayValue getValue () const;

    virtual void setSliderColor (FrameBuffer &fb);

  protected:
    virtual bool hasBorder () const;
    virtual bool isBiPolar() const;
    virtual void onParamValueChanged (const Parameter* param);

  private:
    tDisplayValue m_value;
    bool m_bipolar;
    sigc::connection m_paramValueConnection;
    Parameter *m_param = nullptr;
};

