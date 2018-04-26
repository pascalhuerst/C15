#include <testing/TestDriver.h>
#include "EnumTools.h"

ENUM(FooBar, uint8_t, Foo = 1, Bar = 2, Baz, Bum = 100, Boing = 1 << 3, Knall);

struct EnumRegistryTest
{
    static void registerTests()
    {
      g_test_add_func("/EnumRegistry/string2Enum", []
      {
        g_assert(toFooBar("Foo") == FooBar::Foo);
        g_assert(toFooBar("Bar") == FooBar::Bar);
        g_assert(toFooBar("Baz") == FooBar::Baz);
        g_assert(toFooBar("Bum") == FooBar::Bum);
        g_assert(toFooBar("Boing") == FooBar::Boing);
        g_assert(toFooBar("Knall") == FooBar::Knall);
      });

      g_test_add_func("/EnumRegistry/enum2String", []
      {
        g_assert(toString(FooBar::Foo) == "Foo");
        g_assert(toString(FooBar::Bar) == "Bar");
        g_assert(toString(FooBar::Baz) == "Baz");
        g_assert(toString(FooBar::Bum) == "Bum");
        g_assert(toString(FooBar::Boing) == "Boing");
        g_assert(toString(FooBar::Knall) == "Knall");
      });
  }
};

static TestDriver<EnumRegistryTest> d;

