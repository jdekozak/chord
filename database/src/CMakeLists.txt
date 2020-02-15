include_directories(${CHORD_SOURCE_DIR}/3rdparty/rapidjson/include)
add_library(database chord_db_parser.cpp chord_db_file_reader.cpp)
