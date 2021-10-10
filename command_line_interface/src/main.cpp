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

#include <tohoc/chord/loader/loader.hpp>

#include <memory>

namespace {

tohoc::chord::application::Controller makeController()
{
    return tohoc::chord::application::Controller
    {
        std::make_unique<tohoc::chord::loader::Loader>()
    };
}

}


int main()
{
    makeController().run(R"(../3rdparty/chords-db/src/db/guitar/chords/)");
    return 0;
}
