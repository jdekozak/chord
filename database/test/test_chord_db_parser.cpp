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

TEST_F(TestChordDatabaseParser, Validity)
{
    EXPECT_FALSE(_parser.isValid(""));
}

}}}}

