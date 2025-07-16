#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#include "epics/public_cast20.hpp"

class C
{
public:
    C(int val): x{val}
    {}

private:
    int x;

    bool f() const
    {
        return true;
    }
};

template struct eps::public_access<class CxSecret, &C::x>;
template struct eps::public_access<class CfSecret, &C::f>;

TEST_CASE("testing public_cast")
{
    C c{42};

    SUBCASE("testing public_cast for member variable")
    {
        CHECK(c.*eps::public_cast<int C::*, CxSecret>::m == 42);
    }

    SUBCASE("testing public_cast for member function")
    {
        CHECK((c.*eps::public_cast<bool (C::*)() const, CfSecret>::m)() == true);
    }
}
