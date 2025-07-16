# epics

EPICS is a collection of C++ header-only libraries for things that others told
you should've written yourself

## Usage

Each of the provided libraries consists of a single header file situated in the
`include/epics` subdirectory. The headers can be used with a drop-in approach,
however a CMakeLists.txt is present providing the `epics` target for easier
usage with a build system.

## Naming

Each header file's name consists of a few parts following a certain convention:
`<library-name><cxx-version>.hpp`, where \<cxx-version\> is a number
representing the minimum C++ standard version the library will work with, this
is also the C++ standerd the tests for this library are utilizing.

The following table contains brief information about the minimum C++ standarard
version required for each of the libraries in EPICS

| Header            | Minimum C++ standard |
|-------------------|----------------------|
| operator_in11.hpp | C++11                |
| pstream17.hpp     | C++17                |
| public_cast20.hpp | C++20                |

*operator_in11.hpp* provides a macro analog of an operator `in` that, given a
value and a container, returns a boolean indicating whether the value occurs in
the container.

*pstream17.hpp* provides wrappers for stream types for thread-safe I/O, as well
as wrapper instances for standard I/O streams.

*public_cast20.hpp* provides templates for accessing private class members.
