/*
 * Copyright (C) 2020  Julien de KOZAK jdekozak@hotmail.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <tohoc/chord/database/chord_db_file_reader.hpp>

#include <gtest/gtest.h>

#include <fstream>


namespace tohoc { namespace chord { namespace database { namespace test {

class TestChordDatabaseFileReader : public testing::Test
{
public:
    TestChordDatabaseFileReader() :
        _reader()
    {
        std::remove("C#.json");
    }

    ChordDatabaseFileReader _reader;
};

TEST_F(TestChordDatabaseFileReader, InvalidStream)
{
    std::ifstream input("C#.json");
    ASSERT_THROW(_reader.read(input), std::runtime_error);
}

TEST_F(TestChordDatabaseFileReader, ReadContent)
{
    {
        std::ofstream output("C#.json");
        output << R"({"key":"C#"})";
    }
    std::ifstream input("C#.json");
    EXPECT_EQ(R"({"key":"C#"})", _reader.read(input));
}

}}}}
