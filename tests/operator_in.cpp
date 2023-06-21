#include <iostream>
#include <string>
#include <vector>
// #include <cassert>

#include "../include/eps/operator_in.hpp"

int main(int argc, char* argv[])
{
    if(argc < 3)
    {
        std::cout << "Usage: " << argv[0]
                  << " <keyword> <searchwords>\n";
        return 1;
    }

    std::vector<std::string> v;
    v.reserve(argc - 2);
    for(int i = 2; i < argc; ++i)
    {
        v.emplace_back(argv[i]);
    }

    if
    (
        !((std::string(argv[1]) in v).empty())
    )
    {
        std::cout << "Keyword " << argv[1]
                  << " was found in the following searchwords:\n";
    }
    else
    {
        std::cout << "Keyword " << argv[1]
                  << " was NOT found in the following searchwords:\n";
    }
    for(auto s : v)
    {
        std::cout << s << '\n';
    }

    return 0;
}