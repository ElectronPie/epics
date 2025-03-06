cmake_minimum_required(VERSION 3.15...3.31)

project(epics
    DESCRIPTION "EPICS is a collection of C++ header-only libraries for things that others told you should've written yourself"
    LANGUAGES CXX)

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

    # Let's nicely support folders in IDEs
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

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

    # Docs only available if this is the main app
    find_package(Doxygen)
    if(Doxygen_FOUND)
        set(DOXYGEN_EXTRACT_ALL YES)
        set(DOXYGEN_BUILTIN_STL_SUPPORT YES)
        set(DOXYGEN_EXCLUDE_PATTERNS "build")
        set(DOXYGEN_OUTPUT_DIRECTORY docs)
        set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "README.md")
        set(DOXYGEN_RECURSIVE YES)

        set(epics_DOCS_SHOW_INTERNAL NO CACHE BOOL "Include internal documentation")
        if(epics_DOCS_SHOW_INTERNAL)
            list(APPEND DOXYGEN_ENABLED_SECTIONS SHOW_INTERNAL)
        endif()

        doxygen_add_docs(docs
            README.md include)
    else()
        message(STATUS "Doxygen not found, not building docs")
    endif()
endif()
if((MAIN_PROJECT OR epics_BUILD_TESTING) AND BUILD_TESTING)
    add_subdirectory(tests)
endif()
