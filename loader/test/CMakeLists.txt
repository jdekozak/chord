include_directories(${CHORD_SOURCE_DIR}/loader/include)
include_directories(${CHORD_SOURCE_DIR}/database/include)
include_directories(${CHORD_SOURCE_DIR}/application/include)

add_executable(testLoader
               main.cpp
               test_loader.cpp)

target_link_libraries(testLoader database loader)
target_link_libraries(testLoader gtest gtest_main gmock gmock_main)
target_link_libraries(testLoader stdc++fs)

add_test(NAME loader
         COMMAND testLoader --gtest_output=xml:testLoader.xml )
