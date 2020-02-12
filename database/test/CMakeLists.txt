add_executable(testDatabase main.cpp)

target_link_libraries(testDatabase gtest gtest_main)

add_test(NAME database
         COMMAND testDatabase --gtest_output=xml:testDatabase.xml)
