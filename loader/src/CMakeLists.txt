include_directories(${CHORD_SOURCE_DIR}/database/include)

add_executable(loadDatabase
               main.cpp)

target_link_libraries(loadDatabase database)
