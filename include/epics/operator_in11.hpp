#pragma once

#include <algorithm>

namespace eps
{
    /// @cond SHOW_INTERNAL
    /**
     * @brief Holds a reference to a value before it's checked for appearing in a container.
     *
     * @tparam T The value type
     */
    template<typename T>
    struct __operator_in_lhs
    {
    public:
        /**
         * @brief Construct a new __operator_in_lhs object.
         *
         * @param val The value reference.
         */
        __operator_in_lhs(const T& val): val{val}
        {}

        const T& val; ///< The value reference
    };

    /**
     * @brief Checks whether a value from __operator_in_lhs<T> occurs in a container.
     *
     * @tparam T The value type.
     * @tparam C The container type.
     * @param lhs __operator_in_lhs<T> struct hosting the value.
     * @param c The container.
     * @return bool If the value occurs in the container.
     */
    template<typename T, typename C>
    bool operator|(__operator_in_lhs<T> lhs, const C& c)
    {
        return std::find(std::begin(c), std::end(c), lhs.val) != std::end(c);
    }

    /**
     * @brief Intermediate struct hosting the operator to wrap a value into __operator_in_lhs.
     */
    struct __operator_in
    {};

    /**
     * @brief Wraps a value into __operator_in_lhs.
     *
     * @tparam T The value type.
     * @param lhs The value.
     * @param rhs __operator_in intermediate struct.
     * @return __operator_in_lhs<T> The wrapped value.
     */
    template<typename T>
    __operator_in_lhs<T> operator|(const T& lhs, __operator_in rhs)
    {
        return __operator_in_lhs{lhs};
    }

    /// @endcond
} // namespace eps

/**
 * @brief The operator in
 *
 * A macro analog of the operator `in` that, given a value and a container,
 * returns a boolean indicating whether the value occurs in the container.
 */
#define in | eps::__operator_in{} |
