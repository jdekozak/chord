add_executable(testDatabase main.cpp test_chord_db_parser.cpp)

target_link_libraries(testDatabase database)
target_link_libraries(testDatabase gtest gtest_main gmock gmock_main)

add_test(NAME database
         COMMAND testDatabase --gtest_output=xml:testDatabase.xml)
