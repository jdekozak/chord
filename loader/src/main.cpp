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

#include <iostream>
#include <fstream>

#include <tohoc/chord/database/chord_db_file_reader.h>
#include <tohoc/chord/database/chord_db_custom_json_adapter.h>
#include <tohoc/chord/database/chord_db_parser.h>


int main(int argc, char** argv)
{
    tohoc::chord::database::ChordDatabaseFileReader reader;
    tohoc::chord::database::ChordDatabaseCustomJsonAdapter adapter;
    tohoc::chord::database::ChordDatabaseParser parser;

    for(int index = 1U; index < argc; ++index)
    {
        std::cout << "File : " << argv[index] << '\n';
        std::ifstream currentChordFile(argv[index]);
        auto content = adapter.adapt(reader.read(currentChordFile));
        if(not parser.isValid(content))
        {
            std::cout << "Report Error : " << parser.reportError(content) << '\n';
            std::cout << "Invalid : " << content << '\n';
            continue;
        }
        auto chord = parser.build(content);
    }
    return 0;
}
