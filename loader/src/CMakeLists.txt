include_directories(${CHORD_SOURCE_DIR}/database/include)
include_directories(${CHORD_SOURCE_DIR}/application/include)

add_library(loader
            loader.cpp)

target_link_libraries(loader database)
