include_directories(${CHORD_SOURCE_DIR}/3rdparty/cxxmidi/include)

add_library(application
            writer.cpp
            controller.cpp)
