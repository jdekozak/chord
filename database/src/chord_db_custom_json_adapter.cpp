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

#include <tohoc/chord/database/chord_db_custom_json_adapter.h>

#include <algorithm>
#include <regex>


namespace tohoc {
namespace chord {
namespace database {

namespace {

bool mustBeRemoved(unsigned char character)
{
    return std::isspace(character)
           or (';' == character);
}

}

std::string ChordDatabaseCustomJsonAdapter::adapt(const std::string& input) const
{
    std::string temporary(input);

    temporary.erase(std::remove_if(temporary.begin(),
                                   temporary.end(),
                                   &mustBeRemoved),
                    temporary.end());

    std::replace(temporary.begin(),
                 temporary.end(),
                 '\'',
                 '"');

    std::regex keyword("([a-zA-Z]*):");

    std::string result;
    std::regex_replace(std::back_inserter(result),
                       std::find(temporary.begin(),
                                 temporary.end(),
                                 '{'),
                       temporary.end(),
                       keyword, R"("$1":)");
    return result;
}

}
}
}
