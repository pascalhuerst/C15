#pragma once

#include "playground.h"
#include <device-settings/DebugLevel.h>
#include <string>
#include <map>
#include <boost/preprocessor/list/enum.hpp>
#include <boost/preprocessor/variadic/to_list.hpp>
#include <boost/preprocessor/list/transform.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/stringize.hpp>

namespace EnumTools
{
  template<typename T>
    struct Assignable
    {
        Assignable(const T &v)
        {
          value = v;
        }

        Assignable &operator=(int i)
        {
          value = (T)i;
          return *this;
        }

        bool operator<(const Assignable &a) const
        {
          return value < a.value;
        }

        T value;
    };

  inline std::string parseEnumKey(std::string in)
  {
    auto pos = in.find_first_of(" =,;\n\r");
    if(pos == string::npos)
      return in;
    return in.substr(0, pos);
  }
}

#define TRANSFORM_VARIADIC(op, data, var...) BOOST_PP_LIST_ENUM(BOOST_PP_LIST_TRANSFORM(op, data, BOOST_PP_VARIADIC_TO_LIST(var)))
#define CREATE_E2S_MAP_ENTRY(d, enumName, enumValue) std::make_pair((EnumTools::Assignable<enumName>)enumName::enumValue, EnumTools::parseEnumKey(#enumValue))
#define GENERATE_E2S_MAP_ENTRIES(enumName, enumValues...) TRANSFORM_VARIADIC(CREATE_E2S_MAP_ENTRY, enumName, enumValues)
#define CREATE_S2E_MAP_ENTRY(d, enumName, enumValue) std::make_pair(EnumTools::parseEnumKey(#enumValue), (EnumTools::Assignable<enumName>)enumName::enumValue)
#define GENERATE_S2E_MAP_ENTRIES(enumName, enumValues...) TRANSFORM_VARIADIC(CREATE_S2E_MAP_ENTRY, enumName, enumValues)

#define ENUM(enumName, type, enums...) \
  enum class enumName : type \
  { \
    enums \
  };\
  \
  inline std::string toString(enumName e) \
  { \
    static std::map<EnumTools::Assignable<enumName>, std::string> values ({ GENERATE_E2S_MAP_ENTRIES(enumName, enums) });\
    auto it = values.find((EnumTools::Assignable<enumName>)e); \
    if(it != values.end()) return it->second;\
    DebugLevel::error("Could not find value", (int)e, "in enum map for", typeid(enumName).name()); \
    return ""; \
  }\
  \
  inline enumName to##enumName(const std::string &e) \
  { \
    static std::map<std::string, EnumTools::Assignable<enumName>> values ({ GENERATE_S2E_MAP_ENTRIES(enumName, enums) });\
    auto it = values.find(e); \
    if(it != values.end()) return it->second.value;\
    DebugLevel::error("Could not find value", e, "in enum map for", typeid(enumName).name()); \
    return (enumName)0; \
  }
