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

#include <tohoc/chord/database/chord_db_parser.h>

#include <rapidjson/document.h>
#include <rapidjson/schema.h>


namespace tohoc { namespace chord { namespace database {

namespace {

template<typename Allocator>
void addStringMember(rapidjson::Value& parent,
                     const std::string& name,
                     const std::string& value,
                     Allocator& allocator)
{
    rapidjson::Value child(rapidjson::kStringType);
    child.SetString(value.c_str(), value.size(), allocator);

    rapidjson::Value member(name.c_str(), name.size(), allocator);

    parent.AddMember(member.Move(), child.Move(), allocator);
}

rapidjson::SchemaDocument makeSchema()
{
    rapidjson::Document document(rapidjson::kObjectType);
    addStringMember(document, "type", "object", document.GetAllocator());

    rapidjson::SchemaDocument schema(document);

    return schema;
}

rapidjson::Document parse(const std::string& jsonChord)
{
    rapidjson::Document document;
    document.Parse(jsonChord.c_str());
    return document;
}

}


bool ChordDatabaseParser::isValid(const std::string& jsonChord) const
{
    auto document = parse(jsonChord);
    if(document.HasParseError())
    {
        return false;
    }

    rapidjson::SchemaValidator validator(makeSchema());
    return document.Accept(validator);
}

std::string ChordDatabaseParser::reportError(const std::string& jsonChord) const
{
    auto document = parse(jsonChord);
    if(document.HasParseError())
    {
        return "Parsing error";
    }

    rapidjson::SchemaValidator validator(makeSchema());
    if(not document.Accept(validator))
    {
        rapidjson::StringBuffer error;
        validator.GetInvalidDocumentPointer().StringifyUriFragment(error);
        return error.GetString();
    }
    throw std::runtime_error("Document is valid !");
}

}}}

