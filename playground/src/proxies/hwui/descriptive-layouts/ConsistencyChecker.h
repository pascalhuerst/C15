#pragma once

#include <ostream>

namespace DescriptiveLayouts
{

  class ConsistencyChecker
  {
    public:
      ConsistencyChecker(std::ostream &out);

      bool checkAll();

    private:
      bool checkLayoutSelectorsUnique();
      bool checkLayoutsContainOnlyKnownControlClasses();
      bool checkEventTargetsAreKnown();
      bool checkStyleSelectorsUseKnownPrimitiveInstances();
      bool checkStyleSelectorsUseKnownControlClasses();
      bool checkAllControlClassesAreUsed();
      bool checkAllStylesAreUsed();

      std::ostream &m_out;
  };

}
