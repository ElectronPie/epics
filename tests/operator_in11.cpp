#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#include <array>
#include <list>
#include <string>
#include <vector>

#include "epics/operator_in11.hpp"

TEST_CASE("testing operator in")
{
    SUBCASE("testing on vector")
    {
        std::vector<std::string> v{"Old", "Macdonald", "had", "a", "farm"};
        SUBCASE("")
        {
            CHECK(true  == ("Old" in v));
        }
        SUBCASE("")
        {
            CHECK(false == ("Young" in v));
        }
    }

    SUBCASE("testing on list")
    {
        std::list<std::string> l{"Old", "Macdonald", "had", "a", "farm"};
        SUBCASE("")
        {
            CHECK(true  == ("had" in l));
        }
        SUBCASE("")
        {
            CHECK(false == ("has" in l));
        }
    }

    SUBCASE("testing on string")
    {
        std::string s{"EIEIO"};
        SUBCASE("")
        {
            CHECK(true  == ('E' in s));
        }
        SUBCASE("")
        {
            CHECK(false == ('e' in s));
        }
    }

    SUBCASE("testing on array")
    {
        std::array<char, 5> a{'E', 'I', 'E', 'I', 'O'};
        SUBCASE("")
        {
            CHECK(true  == ('I' in a));
        }
        SUBCASE("")
        {
            CHECK(false == ('i' in a));
        }
    }

    SUBCASE("testing on C-style array")
    {
        const char cs[] = "EIEIO";
        SUBCASE("")
        {
            CHECK(true  == ('O' in cs));
        }
        SUBCASE("")
        {
            CHECK(false == ('o' in cs));
        }
    }
}
