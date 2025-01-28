#include "doctest/doctest.h"

#include <array>
#include <list>
#include <string>
#include <vector>

#include "epics/operator_in11.hpp"

TEST_CASE("testing operator in") {
    std::vector<std::string> v{"Here", "is", "a", "song", "for", "you", ":"};
    std::string s{"Old Macdonald had a farm!"};
    std::array<char, 5> a{'E', 'I', 'E', 'I', 'O'};
    std::list<std::string> l{"And", "on", "that", "farm", "he", "had", "some", "dogs"};
    char c_style[] = {'E', 'I', 'E', 'I', 'O'};

    CHECK(true ==  (std::string{"Here"} in v));
    CHECK(true == !(std::string{"limerick"} in v));

    CHECK(true ==  ('M' in s));
    CHECK(true == !('z' in s));

    CHECK(true ==  ('E' in a));
    CHECK(true == !('e' in a));
    CHECK(true ==  ('E' in c_style));
    CHECK(true == !('e' in c_style));

    CHECK(true ==  ("dogs" in l));
    CHECK(true == !("cats" in l));

    CHECK(true ==  ('O' in a));
    CHECK(true == !('o' in a));
    CHECK(true ==  ('O' in c_style));
    CHECK(true == !('o' in c_style));
}
