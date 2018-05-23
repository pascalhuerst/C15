#include "ConsistencyChecker.h"
#include "LayoutFactory.h"
#include "ControlRegistry.h"

namespace DescriptiveLayouts
{
  ConsistencyChecker::ConsistencyChecker(std::ostream &out) :
      m_out(out)
  {
  }

  bool ConsistencyChecker::checkAll()
  {
    bool ret = checkLayoutSelectorsUnique();
    ret &= checkLayoutsContainOnlyKnownControlClasses();
    ret &= checkEventTargetsAreKnown();
    ret &= checkStyleSelectorsUseKnownPrimitiveInstances();
    ret &= checkStyleSelectorsUseKnownControlClasses();
    ret &= checkAllControlClassesAreUsed();
    ret &= checkAllStylesAreUsed();
    return ret;
  }

  bool ConsistencyChecker::checkLayoutSelectorsUnique()
  {
    const auto &factory = BoledLayoutFactory::get();

    for(const auto &one : factory.m_layouts)
    {
      for(const auto &other : factory.m_layouts)
      {
        if(&one != &other)
        {
          if(one.selectors.size() == other.selectors.size())
          {
            bool foundDifference = false;

            for(const auto &oneSelector : one.selectors)
            {
              for(const auto &otherSelector : other.selectors)
              {
                if(oneSelector == otherSelector)
                {
                  continue;
                }

                foundDifference = true;
              }
            }

            if(!foundDifference)
            {
              m_out << "Error: Two layouts have identical selectors: " << one.id << " and " << other.id << std::endl;
              return false;
            }
          }
        }
      }
    }

    return true;
  }

  bool ConsistencyChecker::checkLayoutsContainOnlyKnownControlClasses()
  {
    const auto &factory = BoledLayoutFactory::get();

    for(const auto &layout : factory.m_layouts)
    {
      for(const auto &control : layout.controls)
      {
        try
        {
          ControlRegistry::get().find(control.controlClass);
        }
        catch(...)
        {
          m_out << "Layout " << layout.id << " references unknown control class " << control.controlClass << std::endl;
          return false;
        }
      }
    }

    return true;
  }

  bool ConsistencyChecker::checkEventTargetsAreKnown()
  {
    const auto &factory = BoledLayoutFactory::get();

    for(const auto &layout : factory.m_layouts)
    {
      for(const auto &control : layout.controls)
      {
        for(const auto &c : control.eventConnections)
        {
          const auto &targetInstance = c.targetInstance;
          const auto &controlClass = ControlRegistry::get().find(control.controlClass);

          auto findResult = std::find_if(controlClass.primitves.begin(), controlClass.primitves.end(), [&](const auto &i)
          {
            return i.primitiveInstance == targetInstance;
          });

          if(findResult == controlClass.primitves.end())
          {
            m_out << "Event map for layout " << layout.id << " references unknown primitive instance " << targetInstance << std::endl;
          }
        }
      }
    }

    return true;
  }

  bool ConsistencyChecker::checkStyleSelectorsUseKnownPrimitiveInstances()
  {
    return true;
  }

  bool ConsistencyChecker::checkStyleSelectorsUseKnownControlClasses()
  {
    return true;
  }

  bool ConsistencyChecker::checkAllControlClassesAreUsed()
  {
    return true;
  }

  bool ConsistencyChecker::checkAllStylesAreUsed()
  {
    return true;
  }

}
