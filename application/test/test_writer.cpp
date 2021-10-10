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

#include <tohoc/chord/application/writer.hpp>

#include <vector>

#include <gtest/gtest.h>


namespace tohoc { namespace chord { namespace application { namespace test {

namespace {

std::vector<unsigned char> OpenStrings
{
    40, 45, 50, 55, 59, 64
};

}
    
class TestWriter : public testing::Test
{
public:
    TestWriter() :
        writer()
    {
    }

    Writer writer;
};

TEST_F(TestWriter, ToFile)
{
    ASSERT_NO_THROW(writer.toMidiFile(std::vector<MidiChord>{MidiChord{"OpenStrings",OpenStrings}}));
}

}}}}
