cmake_minimum_required(VERSION 3.15...3.31)

project(epics CXX)

add_library(epics INTERFACE)
target_include_directories(epics INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include/")
target_sources(epics INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/include/epics/operator_in11.hpp")

set(MAIN_PROJECT OFF)
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(MAIN_PROJECT ON)
endif()

if(MAIN_PROJECT)
    # Ensures -std=c++xx instead of -std=g++xx
    set(CMAKE_CXX_EXTENSIONS OFF)

    include(FetchContent)
    include(CTest)
endif()

if((MAIN_PROJECT OR epics_BUILD_TESTING) AND BUILD_TESTING)
    add_subdirectory(tests)
endif()
