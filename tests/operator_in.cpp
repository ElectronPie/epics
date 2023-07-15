#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <cassert>

#include "../include/eps/operator_in.hpp"

template<typename T>
inline static bool check_presency(std::pair<T, bool> res, bool invert)
{
    return !invert ?
        res.first != T{} &&  res.second:
        res.first == T{} && !res.second;
}

int main(int argc, char* argv[])
{
    std::initializer_list<std::string> song{
        "Old",
        "McDonald",
        "had",
        "a",
        "farm"
    };
    std::vector<std::string> song_vec{song};
    std::list  <std::string> song_lst{song};

    std::string lyric;
    std::string buzzword;

    lyric = "a";
    buzzword = "bruh";
    assert( (lyric               in song).second);
    assert(!(buzzword            in song).second);
    assert( (std::move(lyric)    in song).second);
    assert(!(std::move(buzzword) in song).second);
    assert( ("a"                 in song).second);
    assert(!("bruh"              in song).second);
    assert( (std::move("a")      in song).second);
    assert(!(std::move("bruh")   in song).second);

    lyric = "a";
    buzzword = "bruh";
    assert( (lyric               in song_vec).second);
    assert(!(buzzword            in song_vec).second);
    assert( (std::move(lyric)    in song_vec).second);
    assert(!(std::move(buzzword) in song_vec).second);
    assert( ("a"                 in song_vec).second);
    assert(!("bruh"              in song_vec).second);
    assert( (std::move("a")      in song_vec).second);
    assert(!(std::move("bruh")   in song_vec).second);

    lyric = "a";
    buzzword = "bruh";
    assert( (lyric               in song_lst).second);
    assert(!(buzzword            in song_lst).second);
    assert( (std::move(lyric)    in song_lst).second);
    assert(!(std::move(buzzword) in song_lst).second);
    assert( ("a"                 in song_lst).second);
    assert(!("bruh"              in song_lst).second);
    assert( (std::move("a")      in song_lst).second);
    assert(!(std::move("bruh")   in song_lst).second);

    assert( (lyric    in std::move(song_lst)).second);
    assert( (lyric    in std::move(song_lst)).second);
    assert(!(buzzword in std::move(song_vec)).second);
    assert(!(buzzword in std::move(song_vec)).second);

    assert( (std::move(lyric)    in std::move(song_vec)).second);
    assert( (std::move(lyric)    in std::move(song_vec)).second);
    assert(!(std::move(buzzword) in std::move(song_lst)).second);
    assert(!(std::move(buzzword) in std::move(song_lst)).second);

    assert( ((1 in std::initializer_list<int>{1, 2, 3}).second));
    assert(!((3 in std::initializer_list<int>{2, 4, 6}).second));
    return 0;
}