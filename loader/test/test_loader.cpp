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

#include <tohoc/chord/loader/loader.hpp>

#include <gtest/gtest.h>

#include <fstream>
#include <filesystem>


namespace tohoc { namespace chord { namespace loader { namespace test {

namespace {

bool isNotIndexFile(const std::filesystem::path& filename)
{
    return filename != "index.js";
}

std::vector<std::ifstream> openChordFileDatabase(const std::string& relativePath)
{
    std::vector<std::ifstream> result;

    for(const auto& key : std::filesystem::directory_iterator(relativePath))
    {
        if(isNotIndexFile(key.path().filename()))
        {
            for(const auto& suffix : std::filesystem::directory_iterator(key))
            {
                if(isNotIndexFile(suffix.path().filename()))
                {
                    result.push_back(std::ifstream(suffix.path()));
                }
            }
        }
    }

    return result;
}

}


class TestLoader : public testing::Test
{
public:
    TestLoader() :
        loader()
    {
    }

    Loader loader;
};

TEST_F(TestLoader, Collection)
{
    auto chordDatabase = openChordFileDatabase(R"(../../../3rdparty/chords-db/src/db/guitar/chords/)");
    auto midiChords = loader.read(chordDatabase);
    ASSERT_EQ(2069U, midiChords.size());
}

}}}}
