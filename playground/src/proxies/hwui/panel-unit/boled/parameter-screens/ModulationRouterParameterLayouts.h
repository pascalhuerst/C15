#pragma once

#include "ParameterLayout.h"

class ModulationRouterParameterLayout2 : public virtual ParameterLayout2
{
public:
  typedef ParameterLayout2 super;
  ModulationRouterParameterLayout2 ();

protected:
  virtual bool onButton (int i, bool down, ButtonModifiers modifiers) override;
};

