#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#include <algorithm>
#include <array>
#include <atomic>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "epics/pstream17.hpp"

constexpr unsigned int default_num_threads = 5;
constexpr std::size_t same_values_in_a_row = 3;
using data_type = unsigned int;

std::atomic<bool> go = false;

template<std::size_t... Is>
inline std::array<data_type, same_values_in_a_row> pistream_work(eps::pistream<std::stringstream> pistream, std::index_sequence<Is...>)
{
    std::array<data_type, same_values_in_a_row> readings;
    (pistream >> ... >> readings[Is]);
    return readings;
}

void do_pistream_work(eps::pistream<std::stringstream> pistream, std::vector<bool>::reference result)
{
    while(!go);
    std::array<data_type, same_values_in_a_row> readings =
        pistream_work(std::move(pistream), std::make_index_sequence<same_values_in_a_row>{});
    result = std::adjacent_find(
        readings.cbegin(),
        readings.cend(),
        std::not_equal_to<data_type>{}
    ) == readings.cend();
}

template<std::size_t... Is>
inline void postream_work(data_type n, eps::postream<std::stringstream> postream, std::index_sequence<Is...>)
{
    (postream << ... << (Is, std::to_string(n).append(" ")));
}

void do_postream_work(data_type n, eps::postream<std::stringstream> postream)
{
    while(!go);
    postream_work(n, std::move(postream), std::make_index_sequence<same_values_in_a_row>{});
}

template<std::size_t... Is>
inline std::array<data_type, same_values_in_a_row> pstream_work(data_type n, eps::pstream<std::stringstream, std::stringstream> pstream, std::index_sequence<Is...>)
{
    std::array<data_type, same_values_in_a_row> readings;
    ((pstream << ... << (Is, std::to_string(n).append(" "))) >> ... >> readings[Is]);
    return readings;
}

void do_pstream_work(data_type n, eps::pstream<std::stringstream, std::stringstream> pstream, std::vector<bool>::reference result)
{
    using namespace std::placeholders;

    while(!go);
    std::array<data_type, same_values_in_a_row> readings =
        pstream_work(n, std::move(pstream), std::make_index_sequence<same_values_in_a_row>{});
    result = std::all_of(
        readings.cbegin(),
        readings.cend(),
        std::bind(std::equal_to<data_type>{}, _1, n)
    );
}

TEST_CASE("testing pstream")
{
    unsigned int num_threads = std::thread::hardware_concurrency();
    num_threads = num_threads == 0 ? default_num_threads : num_threads;

    REQUIRE(num_threads > 1);   // Testing is to be done with multiple threads
    REQUIRE(same_values_in_a_row > 1);  // Must be done for program's correct behavior

    std::vector<std::thread> threads;

    {
        std::stringstream ss;
        for(data_type i = 0; i < num_threads; ++i)
        {
            for(std::size_t j = 0; j < same_values_in_a_row; ++j)
            {
                ss << i << " ";
            }
        }
        std::vector<bool> results(num_threads, false);
        threads.reserve(num_threads);
        for(unsigned int i = 0; i < num_threads; ++i)
        {
            threads.emplace_back(do_pistream_work, eps::pistream{ss}, results[i]);
        }
        go = true;
        for(std::thread& t : threads)
        {
            t.join();
        }
        go = false;
        threads.clear();
        CHECK(std::all_of(
            results.cbegin(),
            results.cend(),
            [](bool x) constexpr -> bool { return x; }
        ));
    }

    {
        std::stringstream ss;
        std::vector<bool> results(num_threads, false);
        threads.reserve(num_threads);
        for(unsigned int i = 0; i < num_threads; ++i)
        {
            threads.emplace_back(do_postream_work, i, eps::postream{ss});
        }
        go = true;
        for(std::thread& t : threads)
        {
            t.join();
        }
        go = false;
        threads.clear();
        for(unsigned int i = 0; i < num_threads; ++i)
        {
            std::array<data_type, same_values_in_a_row> readings;
            for(std::size_t j = 0; j < same_values_in_a_row; ++j)
            {
                ss >> readings[j] >> std::ws;
            }
            results[i] = std::adjacent_find(
                readings.cbegin(),
                readings.cend(),
                std::not_equal_to<data_type>{}
            ) == readings.cend();
        }
        CHECK(std::all_of(
            results.cbegin(),
            results.cend(),
            [](bool x) -> bool { return x; }
        ));
        CHECK(ss.eof());
    }

    {
        std::stringstream ss;
        std::vector<bool> results(num_threads, false);
        threads.reserve(num_threads);
        for(unsigned int i = 0; i < num_threads; ++i)
        {
            threads.emplace_back(do_pstream_work, i, eps::pstream<std::stringstream, std::stringstream>{ss, ss}, results[i]);
        }
        go = true;
        for(std::thread& t : threads)
        {
            t.join();
        }
        go = false;
        threads.clear();
        CHECK(std::all_of(
            results.cbegin(),
            results.cend(),
            [](bool x) -> bool { return x; }
        ));
    }
}
