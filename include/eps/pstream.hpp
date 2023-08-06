#pragma once

#if !(defined __cplusplus) || __cplusplus < 201703L
#error EPICS pstream.hpp requires C++ standard version 17 or higher to work properly!
#endif

#include <list>
#include <functional>
#include <mutex>
#include <algorithm>
#include <iterator>
#include <unordered_map>
#include <iostream>

#include "aux_type_traits.hpp"

namespace eps
{
    namespace
    {
        inline std::mutex mgmt_m;
        inline std::unordered_map<void*, std::pair<std::size_t, std::mutex>> refd_muts;
    }

    template<class istream_t>
    class pistream
    {
    private:
        class __temp_pistream
        {
        private:
            std::list<std::function<void()>> l;
            istream_t& m_istream;
            static inline std::mutex m{};

        public:
            __temp_pistream(istream_t& istream):
                m_istream{istream}
            {}

            __temp_pistream(__temp_pistream& rhs):
                m_istream{rhs.m_istream}
            {
                std::move(std::begin(rhs.l), std::end(rhs.l), std::back_inserter(l));
                rhs.l.clear();
            }

            ~__temp_pistream()
            {
                std::lock_guard<std::mutex> lk{refd_muts[(void*)&m_istream].second};
                while(!l.empty())
                {
                    l.front()();
                    l.pop_front();
                }
            }

            template<typename T>
            __temp_pistream operator>>(T& val)
            {
                l.emplace_back(
                    [&m_istream = this->m_istream, &val]() mutable {
                        m_istream >> val;
                    }
                );
                return *this;
            }
        };

        istream_t& m_istream;

    public:
        pistream(istream_t& istream):
            m_istream{istream}
        {
            std::lock_guard<std::mutex> lk{mgmt_m};
            ++refd_muts[(void*)&m_istream].first;
        }

        ~pistream()
        {
            std::lock_guard<std::mutex> lk{mgmt_m};
            if(--refd_muts[(void*)&m_istream].first <= 0)
            {
                refd_muts.erase((void*)&m_istream);
            }
        }

        template<typename T>
        __temp_pistream operator>>(T& val)
        {
            return __temp_pistream{m_istream} >> val;
        }
    };

    template<class ostream_t>
    class postream
    {
    private:
        class __temp_postream
        {
        private:
            ostream_t& m_ostream;
            std::list<std::function<void()>> l;
            static inline std::mutex m{};

        public:
            __temp_postream(ostream_t& ostream):
                m_ostream{ostream}
            {}

            __temp_postream(__temp_postream& rhs):
                m_ostream{rhs.m_ostream}
            {
                std::move(std::begin(rhs.l), std::end(rhs.l), std::back_inserter(l));
                rhs.l.clear();
            }

            ~__temp_postream()
            {
                std::lock_guard<std::mutex> lk{refd_muts[(void*)&m_ostream].second};
                while(!l.empty())
                {
                    l.front()();
                    l.pop_front();
                }
            }

            template<typename T>
            __temp_postream operator<<(const T& val)
            {
                l.emplace_back(
                    [&m_ostream = this->m_ostream, &val]() mutable {
                        m_ostream << val;
                    }
                );
                return *this;
            }

            template<typename T>
            auto operator<<(T&& val) ->
            std::enable_if_t<
                !std::is_const_v<
                    std::remove_reference_t<T>
                > &&
                std::is_rvalue_reference_v<T>,
                __temp_postream
            >
            {
                l.emplace_back(
                    [&m_ostream = this->m_ostream, &val = std::forward<T>(val)]() mutable {
                        m_ostream << std::forward<T>(val);
                    }
                );
                return *this;
            }
        };

        ostream_t& m_ostream;

    public:
        postream(ostream_t& ostream):
            m_ostream{ostream}
        {
            std::lock_guard<std::mutex> lk{mgmt_m};
            ++refd_muts[(void*)&m_ostream].first;
        }

        ~postream()
        {
            std::lock_guard<std::mutex> lk{mgmt_m};
            if(--refd_muts[(void*)&m_ostream].first <= 0)
            {
                refd_muts.erase((void*)&m_ostream);
            }
        }

        template<typename T>
        __temp_postream operator<<(const T& val)
        {
            return __temp_postream{m_ostream} << val;
        }

        template<typename T>
        auto operator<<(T&& val) ->
        std::enable_if_t<
            !std::is_const_v<
                std::remove_reference_t<T>
            > &&
            std::is_rvalue_reference_v<T>,
            __temp_postream
        >
        {
            return __temp_postream{m_ostream} << std::forward<T>(val);
        }
    };

    template<class istream_t, class ostream_t>
    class pstream
    {
    private:
        class __temp_pstream
        {
        private:
            istream_t& m_istream;
            ostream_t& m_ostream;
            std::list<std::function<void()>> l;
            static inline std::mutex m{};

        public:
            __temp_pstream(istream_t& istream, ostream_t& ostream):
                m_istream{istream},
                m_ostream{ostream}
            {}

            __temp_pstream(__temp_pstream& rhs):
                m_istream{rhs.m_istream},
                m_ostream{rhs.m_ostream}
            {
                std::move(std::begin(rhs.l), std::end(rhs.l), std::back_inserter(l));
                rhs.l.clear();
            }

            ~__temp_pstream()
            {
                std::lock_guard<std::mutex> lk{refd_muts[(void*)&m_istream].second};
                std::unique_lock<std::mutex> ul{refd_muts[(void*)&m_ostream].second, std::defer_lock};
                if((void*)&m_istream != (void*)&m_ostream) ul.lock();
                while(!l.empty())
                {
                    l.front()();
                    l.pop_front();
                }
            }

            template<typename T>
            __temp_pstream operator>>(T& val)
            {
                l.emplace_back(
                    [&m_istream = this->m_istream, &val]() mutable {
                        m_istream >> val;
                    }
                );
                return *this;
            }

            template<typename T>
            __temp_pstream operator<<(const T& val)
            {
                l.emplace_back(
                    [&m_ostream = this->m_ostream, &val]() mutable {
                        m_ostream << val;
                    }
                );
                return *this;
            }

            template<typename T>
            auto operator<<(T&& val) ->
            std::enable_if_t<
                !std::is_const_v<
                    std::remove_reference_t<T>
                > &&
                std::is_rvalue_reference_v<T>,
                __temp_pstream
            >
            {
                l.emplace_back(
                    [&m_ostream = this->m_ostream, &val = std::forward<T>(val)]() mutable {
                        m_ostream << std::forward<T>(val);
                    }
                );
                return *this;
            }
        };

        istream_t& m_istream;
        ostream_t& m_ostream;

    public:
        pstream(istream_t& stream):
            pstream(stream, stream)
        {}

        pstream(istream_t& istream, ostream_t& ostream):
            m_istream{istream},
            m_ostream{ostream}
        {
            std::lock_guard<std::mutex> lk{mgmt_m};
            ++refd_muts[(void*)&m_istream].first;
            ++refd_muts[(void*)&m_ostream].first;
        }

        ~pstream()
        {
            std::lock_guard<std::mutex> lk{mgmt_m};
            --refd_muts[(void*)&m_istream].first;
            --refd_muts[(void*)&m_ostream].first;
            if(refd_muts[(void*)&m_istream].first == 0)
            {
                refd_muts.erase((void*)&m_istream);
            }
            if((void*)&m_istream != (void*)&m_ostream && refd_muts[(void*)&m_ostream].first == 0)
            {
                refd_muts.erase((void*)&m_ostream);
            }
        }

        template<typename T>
        __temp_pstream operator>>(T& val)
        {
            return __temp_pstream{m_istream, m_ostream} >> val;
        }

        template<typename T>
        __temp_pstream operator<<(const T& val)
        {
            return __temp_pstream{m_istream, m_ostream} << val;
        }

        template<typename T>
        auto operator<<(T&& val) ->
        std::enable_if_t<
            !std::is_const_v<
                std::remove_reference_t<T>
            > &&
            std::is_rvalue_reference_v<T>,
            __temp_pstream
        >
        {
            return __temp_pstream{m_istream, m_ostream} << std::forward<T>(val);
        }
    };

    inline pistream pcin {std::cin};
    inline pistream pwcin{std::wcin};

    inline postream pcout {std::cout};
    inline postream pwcout{std::wcout};

    inline postream pcerr {std::cerr};
    inline postream pwcerr{std::wcerr};

    inline postream pclog {std::clog};
    inline postream pwclog{std::wclog};

    inline pstream pcio {std::cin, std::cout};
    inline pstream pwcio{std::wcin, std::wcout};
}
