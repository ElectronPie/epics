#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#include "epics/enums_as_flags11.hpp"

enum unscoped_enum
{
    unscoped_value1 = 1 << 0,
    unscoped_value2 = 1 << 1,
};

enum class scoped_enum
{
    value1 = 1 << 0,
    value2 = 1 << 1,
};

enum class scoped_enum8_t : std::uint8_t
{
    value1 = 1 << 0,
    value2 = 1 << 1,
};

EPS_ENUM_AS_FLAGS(unscoped_enum)
EPS_ENUM_AS_FLAGS(scoped_enum)
EPS_ENUM_AS_FLAGS(scoped_enum8_t)

TEST_CASE("testing enum_class_flags")
{
    SUBCASE("testing operator|")
    {
        unscoped_enum x = unscoped_value1;
        CHECK((x |= unscoped_value2) == 3);

        scoped_enum y = scoped_enum::value1;
        CHECK(static_cast<int>(y |= scoped_enum::value2) == 3);

        scoped_enum8_t z = scoped_enum8_t::value1;
        CHECK(static_cast<std::uint8_t>(z |= scoped_enum8_t::value2) == 3);
    }

    SUBCASE("testing operator&")
    {
        unscoped_enum x = static_cast<unscoped_enum>(3);
        CHECK((x &= unscoped_value1) == unscoped_value1);

        scoped_enum y = static_cast<scoped_enum>(3);
        CHECK((y &= scoped_enum::value1) == scoped_enum::value1);

        scoped_enum8_t z = static_cast<scoped_enum8_t>(3);
        CHECK((z &= scoped_enum8_t::value1) == scoped_enum8_t::value1);
    }

    SUBCASE("testing operator^")
    {
        unscoped_enum x = unscoped_value1;
        CHECK((x ^= static_cast<unscoped_enum>(3)) == unscoped_value2);

        scoped_enum y = scoped_enum::value1;
        CHECK((y ^= static_cast<scoped_enum>(3)) == scoped_enum::value2);

        scoped_enum8_t z = scoped_enum8_t::value1;
        CHECK((z ^= static_cast<scoped_enum8_t>(3)) == scoped_enum8_t::value2);
    }

    SUBCASE("testing operator~")
    {
        unscoped_enum x = unscoped_value1;
        CHECK(~~x == x);

        scoped_enum y = scoped_enum::value1;
        CHECK(~~y == y);

        scoped_enum8_t z = scoped_enum8_t::value1;
        CHECK(~~z == z);
    }
}
