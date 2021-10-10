include_directories(${CHORD_SOURCE_DIR}/application/include)
include_directories(${CHORD_SOURCE_DIR}/loader/include)

add_executable(chord
               main.cpp)

target_link_libraries(chord application loader)
target_link_libraries(chord stdc++fs)
