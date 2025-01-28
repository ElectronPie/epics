#pragma once

#include <algorithm>
#include <type_traits>

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

    template<typename T, typename C>
    bool operator|(__operator_in_lhs<T> lhs, C&& c)
    {
        auto found = std::find(
            std::begin(std::forward<C>(c)),
            std::end(std::forward<C>(c)),
            lhs.val
        );
        return found != std::end(std::forward<C>(c));
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
