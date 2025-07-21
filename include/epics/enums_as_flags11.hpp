/**
 * @file enums_as_flags11.hpp
 * @author ElectronPie (tima001f@gmail.com)
 * @brief A macro for implementing bitwise operations on enums as flags.
 *
 * @copyright Copyright (c) 2025 ElectronPie
 */

#ifndef EPICS_ENUM_CLASS_FLAGS11_HPP
#define EPICS_ENUM_CLASS_FLAGS11_HPP

#include <type_traits>

#define EPS_ENUM_AS_FLAGS(flags_t)                                                                 \
    constexpr inline flags_t operator|(const flags_t& lhs, const flags_t& rhs)                        \
    {                                                                                                 \
        using underlying_t = typename std::underlying_type<flags_t>::type;                            \
        return static_cast<flags_t>(static_cast<underlying_t>(lhs) | static_cast<underlying_t>(rhs)); \
    }                                                                                                 \
                                                                                                      \
    inline flags_t& operator|=(flags_t& lhs, const flags_t& rhs)                                      \
    {                                                                                                 \
        lhs = lhs | rhs;                                                                              \
        return lhs;                                                                                   \
    }                                                                                                 \
                                                                                                      \
    constexpr inline flags_t operator&(const flags_t& lhs, const flags_t& rhs)                        \
    {                                                                                                 \
        using underlying_t = typename std::underlying_type<flags_t>::type;                            \
        return static_cast<flags_t>(static_cast<underlying_t>(lhs) & static_cast<underlying_t>(rhs)); \
    }                                                                                                 \
                                                                                                      \
    inline flags_t& operator&=(flags_t& lhs, const flags_t& rhs)                                      \
    {                                                                                                 \
        lhs = lhs & rhs;                                                                              \
        return lhs;                                                                                   \
    }                                                                                                 \
                                                                                                      \
    constexpr inline flags_t operator^(const flags_t& lhs, const flags_t& rhs)                        \
    {                                                                                                 \
        using underlying_t = typename std::underlying_type<flags_t>::type;                            \
        return static_cast<flags_t>(static_cast<underlying_t>(lhs) ^ static_cast<underlying_t>(rhs)); \
    }                                                                                                 \
                                                                                                      \
    inline flags_t& operator^=(flags_t& lhs, const flags_t& rhs)                                      \
    {                                                                                                 \
        lhs = lhs ^ rhs;                                                                              \
        return lhs;                                                                                   \
    }                                                                                                 \
                                                                                                      \
    constexpr inline flags_t operator~(const flags_t& lhs)                                            \
    {                                                                                                 \
        using underlying_t = typename std::underlying_type<flags_t>::type;                            \
        return static_cast<flags_t>(~static_cast<underlying_t>(lhs));                                 \
    }

#endif // EPS_ENUM_CLASS_FLAGS11_HPP
