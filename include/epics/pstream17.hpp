#pragma once

#include <iostream>
#include <functional>
#include <mutex>
#include <queue>
#include <unordered_map>

namespace eps
{
    class pstream_base
    {
    protected:
        static inline std::mutex g_mtx;
    };

    template<typename istream_t>
    class pistream: private pstream_base
    {
    private:
        class __pistream_temp: pstream_base
        {
        public:
            __pistream_temp(istream_t& istream):
                m_istream{istream}
            {}

            __pistream_temp(__pistream_temp&& other):
                m_istream{other.m_istream},
                m_actions{std::move(other.m_actions)}
            {
                while(!other.m_actions.empty())
                {
                    other.m_actions.pop();
                }
            }

            ~__pistream_temp()
            {
                std::lock_guard<std::mutex> lk{g_mtx};
                while (!m_actions.empty())
                {
                    m_actions.front()();
                    m_actions.pop();
                }
            }

            template<typename T>
            __pistream_temp operator>>(T& val)
            {
                m_actions.emplace([&istream = this->m_istream, &val]() mutable {
                    istream >> val;
                });

                return std::move(*this);
            }

        private:
            istream_t& m_istream;
            std::queue<std::function<void()>> m_actions;
        };

    protected:
        istream_t& m_istream;

    public:
        explicit pistream(istream_t& istream):
            m_istream{istream}
        {}

        template<typename T>
        __pistream_temp operator>>(T& val)
        {
            return __pistream_temp{m_istream} >> val;
        };
    };

    template<typename ostream_t>
    class postream: pstream_base
    {
    private:
        class __postream_temp
        {
        public:
            __postream_temp(ostream_t& ostream):
                m_ostream{ostream}
            {}

            __postream_temp(__postream_temp&& other):
                m_ostream{other.m_ostream},
                m_actions{std::move(other.m_actions)}
            {
                while(!other.m_actions.empty())
                {
                    other.m_actions.pop();
                }
            }

            ~__postream_temp()
            {
                std::lock_guard<std::mutex> lk{g_mtx};
                while (!m_actions.empty())
                {
                    m_actions.front()();
                    m_actions.pop();
                }
            }

            template<typename T>
            __postream_temp operator<<(T& val)
            {
                m_actions.emplace([&ostream = this->m_ostream, &val]() mutable {
                    ostream << val;
                });

                return std::move(*this);
            }

        private:
            ostream_t& m_ostream;
            std::queue<std::function<void()>> m_actions;
        };

    protected:
        ostream_t& m_ostream;

    public:
        explicit postream(ostream_t& ostream):
            m_ostream{ostream}
        {}

        template<typename T>
        __postream_temp operator<<(T& val)
        {
            return __postream_temp{m_ostream} << val;
        }
    };

    template<typename istream_t, typename ostream_t>
    class pstream: private pstream_base
    {
    private:
        class __pstream_temp: private pstream_base
        {
        public:
            __pstream_temp(istream_t& istream, ostream_t& ostream):
                m_istream{istream},
                m_ostream{ostream}
            {}

            __pstream_temp(__pstream_temp&& other):
                m_istream{other.m_istream},
                m_ostream{other.m_ostream},
                m_actions{std::move(other.m_actions)}
            {
                while(!other.m_actions.empty())
                {
                    other.m_actions.pop();
                }
            }

            ~__pstream_temp()
            {
                std::lock_guard<std::mutex> lk{g_mtx};
                while(!m_actions.empty())
                {
                    m_actions.front()();
                    m_actions.pop();
                }
            }

            template<typename T>
            __pstream_temp operator>>(T& val)
            {
                m_actions.emplace([&istream = this->m_istream, &val]() mutable {
                    istream >> val;
                });

                return std::move(*this);
            }

            template<typename T>
            __pstream_temp operator<<(T& val)
            {
                m_actions.emplace([&ostream = this->m_ostream, &val]() mutable {
                    ostream << val;
                });

                return std::move(*this);
            }

        private:
            istream_t& m_istream;
            ostream_t& m_ostream;
            std::queue<std::function<void()>> m_actions;
        };

    protected:
        istream_t& m_istream;
        ostream_t& m_ostream;

    public:
        explicit pstream(istream_t& istream, ostream_t& ostream):
            m_istream{istream},
            m_ostream{ostream}
        {}

        operator pistream<istream_t>()
        {
            return pistream{m_istream};
        }

        operator postream<ostream_t>()
        {
            return postream{m_ostream};
        }

        template<typename T>
        __pstream_temp operator>>(T& val)
        {
            return __pstream_temp{m_istream, m_ostream} >> val;
        }

        template<typename T>
        __pstream_temp operator<<(T& val)
        {
            return __pstream_temp{m_istream, m_ostream} << val;
        }
    };

    inline pistream<std::istream> pcin{std::cin};
    inline pistream<std::wistream> pwcin{std::wcin};

    inline postream<std::ostream> pcout{std::cout};
    inline postream<std::wostream> pwcout{std::wcout};

    inline postream<std::ostream> pcerr{std::cerr};
    inline postream<std::wostream> pwcerr{std::wcerr};

    inline postream<std::ostream> pclog{std::clog};
    inline postream<std::wostream> pwclog{std::wclog};

    inline pstream<std::istream, std::ostream> pcio{std::cin, std::cout};
    inline pstream<std::wistream, std::wostream> pwcio{std::wcin, std::wcout};
} // namespace eps
