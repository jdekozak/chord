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

#pragma once

#include <tohoc/chord/application/midi_chord.hpp>

#include <fstream>
#include <vector>
#include <memory>


namespace tohoc { namespace chord { namespace application {

class Controller
{
public:
    class Loader
    {
    public:
        virtual std::vector<MidiChord> read(std::vector<std::ifstream>& paths) const = 0;

        virtual ~Loader() = default;
    };

    explicit Controller(std::unique_ptr<Loader> loader);

    void run(const std::string& chordDatabasePath) const;

private:
    std::unique_ptr<Loader> loader;
};

}}}
