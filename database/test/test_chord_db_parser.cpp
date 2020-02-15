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

#include <tohoc/chord/database/chord_db_parser.h>

#include <gtest/gtest.h>


namespace tohoc { namespace chord { namespace database { namespace test {

class TestChordDatabaseParser : public testing::Test
{
public:
    TestChordDatabaseParser() :
        _parser()
    {
    }

    ChordDatabaseParser _parser;
};

TEST_F(TestChordDatabaseParser, Invalid)
{
    EXPECT_FALSE(_parser.isValid(""));
    EXPECT_EQ("The document is empty.", _parser.reportError(""));

    EXPECT_FALSE(_parser.isValid("[]"));
    EXPECT_EQ("#", _parser.reportError("[]"));

    EXPECT_FALSE(_parser.isValid("{}"));
    EXPECT_EQ("#", _parser.reportError("{}"));

    EXPECT_FALSE(_parser.isValid(R"({"key":"C","suffix": "major"})"));
    EXPECT_EQ("#", _parser.reportError(R"({"key":"C","suffix": "major"})"));

    EXPECT_FALSE(_parser.isValid(R"({"key":"C","positions": []})"));
    EXPECT_EQ("#/positions", _parser.reportError(R"({"key":"C","positions": []})"));
    
    EXPECT_FALSE(_parser.isValid(R"({"suffix":"major","positions": []})"));
    EXPECT_EQ("#/positions", _parser.reportError(R"({"suffix":"major","positions": []})"));
}

TEST_F(TestChordDatabaseParser, ValidCSharpMajor)
{
    auto CSharpMajor = R"(
{
    "key": "C#",
    "suffix": "major",
    "positions": [
        {
            "frets": "x43121",
            "fingers": "043121",
            "barres": 1
        },
        {
            "frets": "446664",
            "fingers": "112341",
            "barres": 4,
            "capo": true
        },
        {
            "frets": "986669",
            "fingers": "321114",
            "barres": 6
        },
        {
            "frets": "9bba99",
            "fingers": "134211",
            "barres": 9,
            "capo": true
        }
    ]
}
)";
    EXPECT_TRUE(_parser.isValid(CSharpMajor));

    auto chord = _parser.build(CSharpMajor);
    EXPECT_EQ("C#", chord.key);
    EXPECT_EQ("major", chord.suffix);
    EXPECT_EQ(4U, chord.positions.size());

    auto firstPosition = chord.positions.front();
    EXPECT_EQ("x43121", firstPosition.frets);
    EXPECT_EQ("043121", firstPosition.fingers);
    EXPECT_EQ(1U, firstPosition.barres);
    EXPECT_FALSE(firstPosition.capo);

    auto lastPosition = chord.positions.back();
    EXPECT_EQ("9bba99", lastPosition.frets);
    EXPECT_EQ("134211", lastPosition.fingers);
    EXPECT_EQ(9U, lastPosition.barres);
    EXPECT_TRUE(lastPosition.capo);
}

}}}}
