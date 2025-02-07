# Testing Library
FetchContent_Declare(
    doctest
    GIT_REPOSITORY https://github.com/doctest/doctest.git
    GIT_TAG v2.4.11
)
FetchContent_MakeAvailable(doctest)
# Adds doctest::doctest

add_executable(operator_in11 operator_in11.cpp)
target_compile_features(operator_in11 PRIVATE cxx_std_11)
target_link_libraries(operator_in11 PRIVATE epics doctest::doctest)
add_test(NAME operator_in11_test COMMAND operator_in11)

add_executable(pstream17 pstream17.cpp)
target_compile_features(pstream17 PRIVATE cxx_std_17)
target_link_libraries(pstream17 PRIVATE epics doctest::doctest)
add_test(NAME pstream17_test COMMAND pstream17)
