#pragma once

#include <proxies/hwui/HWUIEnums.h>
#include <functional>
#include <variant>

namespace DescriptiveLayouts
{
  class Selector
  {
      using Lambda = std::function<bool()>;
      using Criteria = std::variant<UIFocus, UIMode, UIFocusAndModeDetail, Lambda>;

      struct Tester
      {
          FocusAndMode fam;

          Tester(FocusAndMode fam) :
              fam(fam)
          {
          }
          bool operator()(UIFocus f)
          {
            return f == fam.focus;
          }

          bool operator()(UIMode m)
          {
            return m == fam.mode;
          }

          bool operator ()(UIFocusAndModeDetail d)
          {
            return d == fam.detail;
          }

          bool operator()(Lambda l)
          {
            return l();
          }
      };

    public:
      Selector(Criteria c) :
          criteria(c)
      {
      }

      bool test(FocusAndMode fam) const
      {
        return std::visit(Tester(fam), criteria);
      }

    private:
      Criteria criteria;
  };
}
