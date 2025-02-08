cmake_minimum_required(VERSION 3.15...3.31)

project(epics CXX)

add_library(epics INTERFACE)
target_include_directories(epics INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include/")
target_sources(epics INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/include/epics/operator_in11.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/epics/pstream17.hpp")

set(MAIN_PROJECT OFF)
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(MAIN_PROJECT ON)
endif()

if(MAIN_PROJECT)
    # Ensures -std=c++xx instead of -std=g++xx
    set(CMAKE_CXX_EXTENSIONS OFF)

    include(FetchContent)
    include(CTest)

    # Additional target to perform clang-format run, requires clang-format
    find_program(CLANG_FORMAT_EXE clang-format)
    if(CLANG_FORMAT_EXE)
        # Get all project files
        file(GLOB_RECURSE ALL_SOURCE_FILES
            ${CMAKE_CURRENT_SOURCE_DIR}/include/epics/*.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp)

        add_custom_target(clang-format
            COMMAND
                "${CLANG_FORMAT_EXE}"
                -i
                ${ALL_SOURCE_FILES}
            VERBATIM)
    endif()
endif()

if((MAIN_PROJECT OR epics_BUILD_TESTING) AND BUILD_TESTING)
    add_subdirectory(tests)
endif()
