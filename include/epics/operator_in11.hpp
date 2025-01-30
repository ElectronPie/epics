#pragma once

#include <algorithm>


namespace eps
{
    template<typename T>
    struct __operator_in_lhs
    {
    public:
        __operator_in_lhs(const T& val):
            val{val}
        {}

        const T& val;
    };

    template<
        typename T,
        typename C,
        typename... Args
    > bool operator|(__operator_in_lhs<T> lhs, const C& c)
    {
        auto found = std::find(
            std::begin(c),
            std::end(c),
            lhs.val
        );
        return found != std::end(c);
    }

    struct __operator_in
    {};

    template<typename T>
    __operator_in_lhs<T> operator|(const T& lhs, __operator_in rhs)
    {
        return __operator_in_lhs{lhs};
    }
} // namespace eps

#define in | eps::__operator_in{} |
