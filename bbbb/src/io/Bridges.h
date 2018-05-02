#pragma once

#include "bbbb.h"
#include "FromButtonsBridge.h"

class Bridge;

class Bridges
{
  public:
    Bridges();
    virtual ~Bridges();



  private:
    using tBridge = std::unique_ptr<Bridge>;
    std::list<tBridge> m_bridges;

public:
    template <typename t>
    t* getBridge() {
      for(auto& b: m_bridges) {
        auto raw = b.get();
        if(dynamic_cast<FromButtonsBridge*>(raw) != nullptr) {
          return static_cast<t*>(raw);
        }
      }
     return nullptr;
    }

};

