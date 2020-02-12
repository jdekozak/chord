# CMakeLists files in this project can
# refer to the root source directory of the project as ${CHORD_SOURCE_DIR} and
# to the root binary directory of the project as ${CHORD_BINARY_DIR}.

cmake_minimum_required (VERSION 3.13.4)

project (CHORD)

add_subdirectory(./3rdparty/googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(${gmock_SOURCE_DIR}/include ${gmock_SOURCE_DIR})

enable_testing()

add_subdirectory (database)
