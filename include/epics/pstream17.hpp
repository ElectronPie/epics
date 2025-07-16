/**
 * @file pstream17.hpp
 * @author ElectronPie (tima001f@gmail.com)
 * @brief Thread-safe wrappers for input/output streams.
 *
 * @copyright Copyright (c) 2025 ElectronPie
 */

#ifndef EPICS_PSTREAM17_HPP
#define EPICS_PSTREAM17_HPP

#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <unordered_map>

/**
 * @brief Namespace for EPICS library.
 */
namespace eps
{
    /**
     * @brief Base class for the pstreams.
     */
    class pstream_base
    {
    protected:
        static inline std::mutex g_mtx;
    };

    /**
     * @brief Thread-safe wrapper type for input streams.
     *
     * @tparam istream_t The input stream type.
     */
    template<typename istream_t>
    class pistream: private pstream_base
    {
    private:
        /// @cond SHOW_INTERNAL
        /**
         * @brief Accumulates I/O operations to perform atomically.
         */
        class __pistream_temp: pstream_base
        {
        public:
            /**
             * @brief Construct a new pistream temp object.
             *
             * @param istream Reference to the original input stream.
             */
            __pistream_temp(istream_t& istream): m_istream{istream}
            {}

            /**
             * @brief Move constructor.
             *
             * @param other The other __pistream_temp object.
             */
            __pistream_temp(__pistream_temp&& other): m_istream{other.m_istream}, m_actions{std::move(other.m_actions)}
            {
                while (!other.m_actions.empty())
                {
                    other.m_actions.pop();
                }
            }

            /**
             * @brief Destroy the __pistream_temp object performing the accumulated I/O operations atomically.
             */
            ~__pistream_temp()
            {
                std::lock_guard<std::mutex> lk{g_mtx};
                while (!m_actions.empty())
                {
                    m_actions.front()();
                    m_actions.pop();
                }
            }

            /**
             * @brief Add a new I/O operation to the queue.
             *
             * @tparam T The read value type.
             * @param val Stores the value read after performing the action.
             * @return __pistream_temp Moved xvalue of self.
             */
            template<typename T>
            __pistream_temp operator>>(T& val)
            {
                m_actions.emplace([&istream = this->m_istream, &val]() mutable { istream >> val; });

                return std::move(*this);
            }

        private:
            /**
             * @brief Reference to the original input stream.
             */
            istream_t& m_istream;
            /**
             * @brief Queue of the I/O operations performed upon the object destruction.
             */
            std::queue<std::function<void()>> m_actions;
        };

    protected:
        istream_t& m_istream;
        /// @endcond

    public:
        /**
         * @brief Construct a new pistream object.
         *
         * @param istream Reference to the original input stream.
         */
        explicit pistream(istream_t& istream): m_istream{istream}
        {}

        /**
         * @brief Reads a value after the chain of \>\>s is completed.
         *
         * @tparam T The read value type.
         * @param val Reference to the value read.
         * @return __pistream_temp Temporary object to continue the chain of \>\>s.
         */
        template<typename T>
        __pistream_temp operator>>(T& val)
        {
            return __pistream_temp{m_istream} >> val;
        };
    };

    /**
     * @brief Thread-safe wrapper type for output streams.
     *
     * @tparam ostream_t The output stream type.
     */
    template<typename ostream_t>
    class postream: pstream_base
    {
    private:
        /// @cond SHOW_INTERNAL
        /**
         * @brief Accumulates I/O operations to perform atomically.
         */
        class __postream_temp
        {
        public:
            /**
             * @brief Construct a new __postream_temp object.
             *
             * @param ostream Reference to the original output stream.
             */
            __postream_temp(ostream_t& ostream): m_ostream{ostream}
            {}

            /**
             * @brief Move constructor.
             *
             * @param other The other __postream_temp object.
             */
            __postream_temp(__postream_temp&& other): m_ostream{other.m_ostream}, m_actions{std::move(other.m_actions)}
            {
                while (!other.m_actions.empty())
                {
                    other.m_actions.pop();
                }
            }

            /**
             * @brief Destroy the __postream_temp object performing the accumulated I/O operations atomically.
             */
            ~__postream_temp()
            {
                std::lock_guard<std::mutex> lk{g_mtx};
                while (!m_actions.empty())
                {
                    m_actions.front()();
                    m_actions.pop();
                }
            }

            /**
             * @brief Add a new I/O operation to the queue.
             *
             * @tparam T The written value type.
             * @param val The value to write.
             * @return __postream_temp Moved xvalue of self.
             */
            template<typename T>
            __postream_temp operator<<(T& val)
            {
                m_actions.emplace([&ostream = this->m_ostream, &val]() mutable { ostream << val; });

                return std::move(*this);
            }

        private:
            /**
             * @brief Reference to the original output stream.
             */
            ostream_t& m_ostream;
            /**
             * @brief Queue of the I/O operations performed upon the object destruction.
             */
            std::queue<std::function<void()>> m_actions;
        };

    protected:
        /**
         * @brief Reference to the original output stream.
         */
        ostream_t& m_ostream;
        /// @endcond

    public:
        /**
         * @brief Construct a new postream object.
         *
         * @param ostream Reference to the original output stream.
         */
        explicit postream(ostream_t& ostream): m_ostream{ostream}
        {}

        /**
         * @brief Writes a value after the chain of \<\<s is completed.
         *
         * @tparam T The written value type.
         * @param val The value to write.
         * @return __postream_temp Temporary object to continue the chain of \<\<s.
         */
        template<typename T>
        __postream_temp operator<<(T& val)
        {
            return __postream_temp{m_ostream} << val;
        }
    };

    /**
     * @brief Thread-safe wrapper type for input/output streams.
     *
     * @tparam istream_t Input stream interface type.
     * @tparam ostream_t Output stream interface type.
     */
    template<typename istream_t, typename ostream_t>
    class pstream: private pstream_base
    {
    private:
        /// @cond SHOW_INTERNAL
        /**
         * @brief Accumulates I/O operations to perform atomically.
         */
        class __pstream_temp: private pstream_base
        {
        public:
            /**
             * @brief Construct a new __pstream_temp object
             *
             * @param istream Reference to the original input stream.
             * @param ostream Reference to the original output stream.
             */
            __pstream_temp(istream_t& istream, ostream_t& ostream): m_istream{istream}, m_ostream{ostream}
            {}

            /**
             * @brief Move constructor.
             *
             * @param other The other __pstream_temp object.
             */
            __pstream_temp(__pstream_temp&& other):
                m_istream{other.m_istream}, m_ostream{other.m_ostream}, m_actions{std::move(other.m_actions)}
            {
                while (!other.m_actions.empty())
                {
                    other.m_actions.pop();
                }
            }

            /**
             * @brief Destroy the __pstream_temp object performing the accumulated I/O operations atomically.
             */
            ~__pstream_temp()
            {
                std::lock_guard<std::mutex> lk{g_mtx};
                while (!m_actions.empty())
                {
                    m_actions.front()();
                    m_actions.pop();
                }
            }

            /**
             * @brief Add a new I/O operation to the queue.
             *
             * @tparam T The read value type.
             * @param val Reference to the value read.
             * @return __postream_temp Moved xvalue of self.
             */
            template<typename T>
            __pstream_temp operator>>(T& val)
            {
                m_actions.emplace([&istream = this->m_istream, &val]() mutable { istream >> val; });

                return std::move(*this);
            }

            /**
             * @brief Add a new I/O operation to the queue.
             *
             * @tparam T The written value type.
             * @param val The value to write.
             * @return __postream_temp Moved xvalue of self.
             */
            template<typename T>
            __pstream_temp operator<<(T& val)
            {
                m_actions.emplace([&ostream = this->m_ostream, &val]() mutable { ostream << val; });

                return std::move(*this);
            }

        private:
            /**
             * @brief Reference to the original input stream.
             */
            istream_t& m_istream;
            /**
             * @brief Reference to the original output stream.
             */
            ostream_t& m_ostream;
            /**
             * @brief Queue of the I/O operations performed upon the object destruction.
             */
            std::queue<std::function<void()>> m_actions;
        };

    protected:
        /**
         * @brief Reference to the original input stream.
         */
        istream_t& m_istream;
        /**
         * @brief Reference to the original output stream.
         */
        ostream_t& m_ostream;
        ///@endcond

    public:
        /**
         * @brief Construct a new pstream object.
         *
         * @param istream Reference to the original input stream.
         * @param ostream Reference to the original output stream.
         */
        explicit pstream(istream_t& istream, ostream_t& ostream): m_istream{istream}, m_ostream{ostream}
        {}

        /**
         * @brief Cast to pistream<istream_t>.
         *
         * @return pistream<istream_t>
         */
        operator pistream<istream_t>()
        {
            return pistream{m_istream};
        }

        /**
         * @brief Cast to postream<ostream_t>.
         *
         * @return postream<ostream_t>
         */
        operator postream<ostream_t>()
        {
            return postream{m_ostream};
        }

        /**
         * @brief Reads a value after the chain of \<\<s and \>\>s is completed.
         *
         * @tparam T The read value type.
         * @param val Reference to the value read.
         * @return __pistream_temp Temporary object to continue the chain of \<\<s and \>\>s.
         */
        template<typename T>
        __pstream_temp operator>>(T& val)
        {
            return __pstream_temp{m_istream, m_ostream} >> val;
        }

        /**
         * @brief Writes a value after the chain of \<\<s and \>\>s is completed.
         *
         * @tparam T The written value type.
         * @param val The value to write.
         * @return __pstream_temp Temporary object to continue the chain of \<\<s and \>\>s.
         */
        template<typename T>
        __pstream_temp operator<<(T& val)
        {
            return __pstream_temp{m_istream, m_ostream} << val;
        }
    };

    inline pistream<std::istream> pcin{std::cin};    ///< Thread-safe `std::cin` wrapper.
    inline pistream<std::wistream> pwcin{std::wcin}; ///< Thread-safe `std::wcin` wrapper.

    inline postream<std::ostream> pcout{std::cout};    ///< Thread-safe `std::cout` wrapper.
    inline postream<std::wostream> pwcout{std::wcout}; ///< Thread-safe `std::wcout` wrapper.

    inline postream<std::ostream> pcerr{std::cerr};    ///< Thread-safe `std::cerr` wrapper.
    inline postream<std::wostream> pwcerr{std::wcerr}; ///< Thread-safe `std::wcerr` wrapper.

    inline postream<std::ostream> pclog{std::clog};    ///< Thread-safe `std::clog` wrapper.
    inline postream<std::wostream> pwclog{std::wclog}; ///< Thread-safe `std::wclog` wrapper.

    inline pstream<std::istream, std::ostream> pcio{std::cin, std::cout};
    ///<  Thread-safe I/O wrapper for `std::cin` and `std::cout`.
    inline pstream<std::wistream, std::wostream> pwcio{std::wcin, std::wcout};
    ///<  Thread-safe I/O wrapper for `std::wcin` and `std::wcout`.
} // namespace eps

#endif // EPICS_PSTREAM17_HPP
