#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <atomic>

#include "../include/eps/pstream.hpp"

constexpr std::size_t NUM_WORKERS = 5ULL;

std::atomic<bool> go{false};

static void do_work(std::size_t i)
{
    while(!go.load());

    std::cout  <<  "Thread #" << i <<  " writing to std::cout\n";
    // std::wcout << L"Поток №"  << i << L" пишет в std::wcout\n";

    std::clog  <<  "Thread #" << i <<  " writing to std::clog\n";
    // std::wclog << L"Поток №"  << i << L" пишет в std::wclog\n";

    std::cerr  <<  "Thread #" << i <<  " writing to std::cerr\n";
    // std::wcerr << L"Поток №"  << i << L" пишет в std::wcerr\n";

    std::string s;
    std::cout << "Thread #" << i << " is requesting a string input: ";
    std::cin >> s;
    std::cout << "Thread #" << i << " has recieved a string: " << s << '\n';

    // std::wstring ws;
    // std::wcout << L"Поток №" << i << L" запрашивает строковой ввод: ";
    // std::wcin >> ws;
    // std::wcout << L"Поток №" << i << L" получил строку: " << ws;
}

static void do_pwork(std::size_t i)
{
    while(!go.load());

    eps::pcout  <<  "Thread #" << i <<  " writing to eps::pcout\n";
    // eps::pwcout << L"Поток №"  << i << L" пишет в eps::pwcout\n";

    eps::pclog  <<  "Thread #" << i <<  " writing to eps::pclog\n";
    // eps::pwclog << L"Поток №"  << i << L" пишет в eps::pwclog\n";

    eps::pcerr  <<  "Thread #" << i <<  " writing to eps::pcerr\n";
    // eps::pwcerr << L"Поток №"  << i << L" пишет в eps::pwcerr\n";

    std::string ps;
    eps::pcio << "Thread #" << i << " is requesting a string input while using advanced protection: "
              >> ps
              << "Thread #" << i << " has recieved a string: " << ps << '\n';

    // std::wstring pws;
    // eps::pwcio << L"Поток №" << i << L" запрашивает строковой ввод, используя защиту: "
    //            >> pws
    //            << L"Поток №" << i << L" получил строку: " << pws << '\n';

    /*
    std::string s;
    eps::pcout << "Thread #" << i << " is requesting a string input without using advanced protection: ";
    eps::pcin >> s;
    eps::pcout << "Thread #" << i << " has recieved a string: " << s << '\n';

    // std::wstring ws;
    // eps::pwcout << L"Поток №" << i << L" запрашивает строковой ввод, не используя защиту: ";
    // eps::pwcin >> ws;
    // eps::pwcout << L"Поток №" << i << L" получил строку: " << ws;
    */
}

int main(int argc, char* argv[])
{
    std::vector<std::thread> pv;
    pv.reserve(NUM_WORKERS);
    for(std::size_t i = 1; i <= NUM_WORKERS; ++i)
    {
        pv.emplace_back(do_pwork, i);
    }
    go.store(true);
    for(auto& t : pv) t.join();

    std::string do_snapback;
    eps::pcio << "Execute do_work()? " >> do_snapback;
    if(std::tolower(do_snapback[0]) == 'y')
    {
        go.store(false);
        std::vector<std::thread> v;
        v.reserve(NUM_WORKERS);
        for(std::size_t i = 1; i <= NUM_WORKERS; ++i)
        {
            v.emplace_back(do_work, i);
        }
        go.store(true);
        for(auto& t : v) t.join();
    }

    return 0;
}