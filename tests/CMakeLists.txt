# Testing Library
FetchContent_Declare(
    doctest
    GIT_REPOSITORY https://github.com/doctest/doctest.git
    GIT_TAG v2.4.12
)
FetchContent_MakeAvailable(doctest)
# Adds doctest::doctest

add_custom_target(check
    "${CMAKE_CTEST_COMMAND}"
)

add_executable(enums_as_flags11 EXCLUDE_FROM_ALL enums_as_flags11.cpp)
target_compile_features(enums_as_flags11 PRIVATE cxx_std_11)
target_link_libraries(enums_as_flags11 PRIVATE epics doctest::doctest)
add_dependencies(check enums_as_flags11)
add_test(NAME enums_as_flags11_test COMMAND enums_as_flags11)

add_executable(operator_in11 EXCLUDE_FROM_ALL operator_in11.cpp)
target_compile_features(operator_in11 PRIVATE cxx_std_11)
target_link_libraries(operator_in11 PRIVATE epics doctest::doctest)
add_dependencies(check operator_in11)
add_test(NAME operator_in11_test COMMAND operator_in11)

add_executable(pstream17 EXCLUDE_FROM_ALL pstream17.cpp)
target_compile_features(pstream17 PRIVATE cxx_std_17)
target_link_libraries(pstream17 PRIVATE epics doctest::doctest)
add_dependencies(check pstream17)
add_test(NAME pstream17_test COMMAND pstream17)

add_executable(public_cast20 EXCLUDE_FROM_ALL public_cast20.cpp)
target_compile_features(public_cast20 PRIVATE cxx_std_20)
target_link_libraries(public_cast20 PRIVATE epics doctest::doctest)
add_dependencies(check public_cast20)
add_test(NAME public_cast20_test COMMAND public_cast20)
