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

#include <tohoc/chord/application/controller.hpp>

#include <vector>

#include <gtest/gtest.h>
#include <gmock/gmock.h>


namespace tohoc { namespace chord { namespace application { namespace test {

namespace {

class LoaderMock : public Controller::Loader
{
public:
    MOCK_CONST_METHOD1(read, std::vector<MidiChord>(std::vector<std::ifstream>&));
};

std::vector<unsigned char> OpenStrings
{
    40, 45, 50, 55, 59, 64
};

}

class TestController : public testing::Test
{
public:
    TestController() :
        loader(new testing::StrictMock<LoaderMock>),
        controller(std::unique_ptr<Controller::Loader>(loader))
    {
    }

    LoaderMock* loader;
    Controller controller;
};

TEST_F(TestController, Run)
{
    EXPECT_CALL(*loader, read(testing::_)).
        WillOnce(testing::Return(std::vector<MidiChord>{MidiChord{"OpenStrings",OpenStrings}}));

    ASSERT_NO_THROW(controller.run(R"(../../../3rdparty/chords-db/src/db/guitar/chords/)"););
}

}}}}
