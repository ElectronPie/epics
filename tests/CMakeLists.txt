# Testing Library
FetchContent_Declare(
    doctest
    GIT_REPOSITORY https://github.com/doctest/doctest.git
    GIT_TAG v2.4.11
)
FetchContent_MakeAvailable(doctest)
# Adds doctest::doctest

add_executable(test_epics tests.cpp operator_in.cpp)
target_link_libraries(test_epics PRIVATE epics doctest::doctest)
add_test(NAME test_epics_test COMMAND test_epics)
