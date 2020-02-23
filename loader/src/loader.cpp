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

#include <tohoc/chord/loader/loader.h>

#include <array>
#include <algorithm>
#include <fstream>
#include <iostream>

#include <tohoc/chord/database/chord_db_file_reader.h>
#include <tohoc/chord/database/chord_db_custom_json_adapter.h>
#include <tohoc/chord/database/chord_db_parser.h>


namespace tohoc { namespace chord { namespace loader {

namespace {

const uint8_t StringNumber = 6U;

const std::array<unsigned char, StringNumber> StandardGuitarTuning
{
//  E2  A2  D3  G3  B3  E4
    40, 45, 50, 55, 59, 64
};

uint8_t toFretNumber(char fret)
{
    switch(fret)
    {
    case '0':
        return 0;
    case '1':
        return 1;
    case '2':
        return 2;
    case '3':
        return 3;
    case '4':
        return 4;
    case '5':
        return 5;
    case '6':
        return 6;
    case '7':
        return 7;
    case '8':
        return 8;
    case '9':
        return 9;
    case 'a':
        return 10;
    case 'b':
        return 11;
    case 'c':
        return 12;
    case 'd':
        return 13;
    case 'e':
        return 14;
    case 'f':
        return 15;
    case 'g':
        return 16;
    case 'h':
        return 17;
    case 'i':
        return 18;
    case 'j':
        return 19;
    case 'k':
        return 20;
    case 'l':
        return 21;
    case 'm':
        return 22;
    case 'n':
        return 23;
    case 'o':
        return 24;
    default:
        throw std::runtime_error("unexpected value");
    }
}

std::vector<unsigned char> convertToMidi(const database::Position& position)
{
    std::vector<unsigned char> result;
    result.reserve(6U);

    for(uint8_t index = 0U; index < StringNumber; ++index)
    {
        auto fret = position.frets[index];
        if(fret != 'x')
        {
            result.push_back(StandardGuitarTuning[index] + toFretNumber(fret));
        }
    }

    return result;
}

}

std::vector<application::MidiChord> Loader::read(std::vector<std::ifstream>& paths) const
{
    std::vector<application::MidiChord> result;
    result.reserve(paths.size());

    tohoc::chord::database::ChordDatabaseFileReader reader;
    tohoc::chord::database::ChordDatabaseCustomJsonAdapter adapter;
    tohoc::chord::database::ChordDatabaseParser parser;

    for(auto& currentChordFile : paths)
    {
        auto content = adapter.adapt(reader.read(currentChordFile));

        if(not parser.isValid(content))
        {
            std::cerr << "Report Error : " << parser.reportError(content) << '\n';
            std::cerr << "Invalid : " << content << '\n';
            continue;
        }

        auto chord = parser.build(content);
        std::transform(chord.positions.begin(),
                       chord.positions.end(),
                       std::back_inserter(result),
                       [&chord](const auto& position)
                       {
                           return application::MidiChord
                           {
                               chord.key + chord.suffix,
                               convertToMidi(position)
                           };
                       });
    }

    return result;
}

}}}
