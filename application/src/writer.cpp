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

#include <cxxmidi/track.hpp>
#include <cxxmidi/file.hpp>
#include <cxxmidi/converters.hpp>
#include <cxxmidi/note.hpp>
#include <cxxmidi/instrument.hpp>


namespace tohoc { namespace chord { namespace application {

namespace {

void chordOn(uint32_t deltatime, cxxmidi::Track& track, const std::vector<unsigned char>& chord, uint8_t velocity)
{
    track.push_back(cxxmidi::Event(deltatime,
                                   cxxmidi::Message::kNoteOn,
                                   cxxmidi::Note(chord.front()),
                                   velocity));

    std::transform(chord.begin()+1,
                   chord.end(),
                   std::back_inserter(track),
                   [&velocity](const auto note)
                   {
                        return cxxmidi::Event(0,
                                              cxxmidi::Message::kNoteOn,
                                              cxxmidi::Note(note),
                                              velocity);
                   });
}

}


void Writer::toMidiFile(const std::vector<MidiChord>& chords) const
{
    // quartenote deltatime [ticks]
    // What value should dt be, if we want quarter notes to last 0.5s?

    // Default MIDI time division is 500ticks/quarternote.
    // Default MIDI tempo is 500000us per quarternote
    uint32_t deltatime = cxxmidi::converters::Us2dt(500000, // 0.5s
                                    500000, // tempo [us/quarternote]
                                    500); // time division [us/quarternote]

    cxxmidi::File myFile;
    cxxmidi::Track &track = myFile.AddTrack();

    track.push_back(cxxmidi::Event(0,
                                   cxxmidi::Message::kProgramChange,
                                   cxxmidi::Instrument::kAcousticGuitarNylon));

    for(const auto& chord : chords)
    {
        cxxmidi::Event chordName(0,
                                 cxxmidi::Message::kMeta,
                                 cxxmidi::Message::kText);
        chordName.insert(chordName.end(), chord.name.begin(), chord.name.end());
        chordName.push_back(';');

        track.push_back(chordName);
        chordOn(0 , track, chord.chord, 100);
        chordOn(deltatime, track, chord.chord, 0);
    }

    track.push_back(cxxmidi::Event(deltatime,
                                   cxxmidi::Message::kMeta,
                                   cxxmidi::Message::kEndOfTrack));

    myFile.SaveAs("chord.mid");
}

}}}
