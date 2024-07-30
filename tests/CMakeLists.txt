# Copyright 2024 GlitchyByte
# SPDX-License-Identifier: MIT-0

cmake_minimum_required(VERSION 3.26)

set(IS_MACOS CMAKE_SYSTEM_NAME STREQUAL "Darwin")
set(IS_LINUX CMAKE_SYSTEM_NAME STREQUAL "Linux")
set(IS_WINDOWS CMAKE_SYSTEM_NAME STREQUAL "Windows")

if (NOT DEFINED CMAKE_BUILD_TYPE)
    # Force Debug if no build type specified.
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type." FORCE)
endif ()

project(gblib_test VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message(STATUS "Project: ${PROJECT_NAME} v${PROJECT_VERSION}")
message(STATUS "Platform: ${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_PROCESSOR}")

# Set flags.
if (CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
    if (${IS_LINUX} OR ${IS_MACOS})
        # Linux and macOS.
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto -ffunction-sections -fdata-sections")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -flto -ffunction-sections -fdata-sections")
    elseif (${IS_WINDOWS})
        # Windows has not been tested.
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GL")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /GL")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /LTCG")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /LTCG")
    else ()
        message(WARNING "Unknown OS.")
    endif ()
endif ()

# Minimize binary size.
if ((CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "MinSizeRel") AND
(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "Darwin"))
    # Strip all remaining symbols and relocation information.
    add_custom_command(
            TARGET ${PROJECT_NAME}
            POST_BUILD
            COMMAND strip $<TARGET_FILE:${PROJECT_NAME}>
    )
endif ()

include(FetchContent)

# Test framework.
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest
        GIT_TAG v1.15.0
)
FetchContent_MakeAvailable(googletest)

# Origo Proxy.
FetchContent_Declare(
        gblib
        SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/../"
)
FetchContent_MakeAvailable(gblib)

file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS "*.cc")
add_executable(${PROJECT_NAME} ${TEST_SOURCES})

# Add GoogleTest libraries.
target_link_libraries(${PROJECT_NAME}
        PRIVATE gblib
        PRIVATE GTest::gtest_main
)
include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})
