add_executable(testApplication
               main.cpp
               test_writer.cpp
               test_controller.cpp)

target_link_libraries(testApplication application)
target_link_libraries(testApplication gtest gtest_main gmock gmock_main)
target_link_libraries(testApplication stdc++fs)

add_test(NAME application
         COMMAND testApplication --gtest_output=xml:testApplication.xml )
