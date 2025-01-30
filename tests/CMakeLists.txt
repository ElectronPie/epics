# Testing Library
FetchContent_Declare(
    doctest
    GIT_REPOSITORY https://github.com/doctest/doctest.git
    GIT_TAG v2.4.11
)
FetchContent_MakeAvailable(doctest)
# Adds doctest::doctest

add_executable(test_operator_in11 operator_in11.cpp)
target_compile_features(test_operator_in11 PRIVATE cxx_std_11)
target_link_libraries(test_operator_in11 PRIVATE epics doctest::doctest)
add_test(NAME test_operator_in_test11 COMMAND test_operator_in11)
