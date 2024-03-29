#pragma once

#if !(defined __cplusplus) || __cplusplus < 201103L
#error EPICS aux_type_traits.hpp requires C++ standard version 11 or higher to work properly!
#endif

#include <type_traits>
#include <iterator>

namespace eps
{
    template<typename It, typename = void>
    struct is_const_iterator : std::false_type {};

    template<typename It>
    struct is_const_iterator<
        It,
        typename std::enable_if<
            std::is_const<
                typename std::remove_pointer<
                    typename std::iterator_traits<It>::pointer
                >::type
            >::value
        >::type
    > : std::true_type {};

#if __cplusplus >= 201703L
    template<typename It> inline constexpr bool is_const_iterator_v = is_const_iterator<It>::value;
#endif

    template<typename C, typename = void>
    struct is_const_container : std::false_type {};

    template<typename C>
    struct is_const_container<
        C,
        typename std::enable_if<
            is_const_iterator<
                typename std::remove_reference<
                    decltype(std::begin(std::declval<C&>()))
                >::value
            >::value
        >::type
    > : std::true_type {};

#if __cplusplus >= 201703L
    template<typename C> inline constexpr bool is_const_container_v = is_const_container<C>::value;
#endif

#if __cplusplus >= 201703L
    template<typename, typename>
    struct is_same_template_class : std::false_type {};

    template<template<typename...> class T, typename... A, typename... B>
    struct is_same_template_class<T<A...>, T<B...>> : std::true_type {};

    template<class Ta, class Tb> inline constexpr bool is_same_template_class_v = is_same_template_class<Ta, Tb>::value;
#endif

    template<typename C>
    struct element_of
    {
        using type = typename std::remove_reference<decltype(*(std::begin(std::declval<C&>())))>::type;
    };

    template<typename C> using element_of_t = typename element_of<C>::type;
}
