/**
 * @file public_cast20.hpp
 * @author ElectronPie (tima001f@gmail.com)
 * @brief A public_cast template for accessing class members.
 *
 * @copyright Copyright (c) 2025
 */

#ifndef EPICS_PUBLIC_CAST20_HPP
#define EPICS_PUBLIC_CAST20_HPP

/**
 * @brief Namespace for EPICS library.
 */
namespace eps
{
    /**
     * @brief Template for public access to a class member.
     *
     * @tparam T The type of the member.
     * @tparam Secret The secret class that allows access to the member.
     */
    template<typename T, class Secret>
    struct public_cast
    {
        static inline T m{};
    };

    /**
     * @brief Template for creating the Secret class that allows public access to a class member.
     *
     * @tparam Secret The secret class type.
     * @tparam M Class member pointer to be accessed publicly.
     */
    template<class Secret, auto M>
    struct public_access
    {
        static inline const auto m = public_cast<decltype(M), Secret>::m = M;
    };
} // namespace eps

#endif // EPICS_PUBLIC_CAST20_HPP
