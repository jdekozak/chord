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

#include <tohoc/chord/application/midi_chord.h>

#include <fstream>
#include <vector>
#include <memory>


namespace tohoc { namespace chord { namespace application {

class Writer
{
public:
    class Loader
    {
    public:
        virtual std::vector<MidiChord> read(std::vector<std::ifstream>& paths) const = 0;

        virtual ~Loader() = default;
    };

    explicit Writer(std::unique_ptr<Loader> loader);

    void toMidiFile(std::vector<MidiChord>& chords) const;

private:
    std::unique_ptr<Loader> loader;
};

}}}
