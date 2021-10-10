# CMakeLists files in this project can
# refer to the root source directory of the project as ${CHORD_SOURCE_DIR} and
# to the root binary directory of the project as ${CHORD_BINARY_DIR}.

cmake_minimum_required (VERSION 3.12.4)

project (CHORD)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_subdirectory(./3rdparty/googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(${gmock_SOURCE_DIR}/include ${gmock_SOURCE_DIR})

enable_testing()

add_subdirectory (database)
add_subdirectory (loader)
add_subdirectory (application)

add_subdirectory (command_line_interface)
