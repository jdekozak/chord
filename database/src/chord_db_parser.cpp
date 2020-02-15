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
#include <rapidjson/error/en.h>


namespace tohoc { namespace chord { namespace database {

namespace {

template<typename Allocator>
void addStringMember(rapidjson::Value& parent,
                     const std::string& name,
                     const std::string& value,
                     Allocator& allocator)
{
    rapidjson::Value member(rapidjson::kStringType);
    member.SetString(name.c_str(), name.length(), allocator);

    rapidjson::Value child(rapidjson::kStringType);
    child.SetString(value.c_str(), value.length(), allocator);

    parent.AddMember(member, child, allocator);
}

template<typename Allocator>
void addObjectMember(rapidjson::Value& parent,
                     rapidjson::Value& child,
                     const std::string& name,
                     Allocator& allocator)
{
    rapidjson::Value member(rapidjson::kStringType);
    member.SetString(name.c_str(), name.length(), allocator);

    parent.AddMember(member, child, allocator);
}

rapidjson::Document makeSchema()
{
    rapidjson::Document document(rapidjson::kObjectType);
    auto& allocator = document.GetAllocator();

    addStringMember(document, "$id", "https://example.com/person.schema.json", allocator);
    addStringMember(document, "$schema", "http://json-schema.org/draft-07/schema#", allocator);

    addStringMember(document, "title", "chord", allocator);
    addStringMember(document, "type", "object", allocator);

    rapidjson::Value properties(rapidjson::kObjectType);

    rapidjson::Value key(rapidjson::kObjectType);
    addStringMember(key, "type", "string", allocator);
    addObjectMember(properties, key, "key", allocator);

    rapidjson::Value suffix(rapidjson::kObjectType);
    addStringMember(suffix, "type", "string", allocator);
    addObjectMember(properties, suffix, "suffix", allocator);

    rapidjson::Value positions(rapidjson::kObjectType);
    addStringMember(positions, "type", "array", allocator);
    addObjectMember(properties, positions, "positions", allocator);

    addObjectMember(document, properties, "properties", allocator);

    return document;
}

rapidjson::Document parse(const std::string& jsonChord)
{
    rapidjson::Document document;
    document.Parse(jsonChord.c_str());
    return document;
}

std::string getErrorAsString(const rapidjson::SchemaValidator& validator)
{
    rapidjson::StringBuffer error;
    validator.GetInvalidDocumentPointer().StringifyUriFragment(error);
    return error.GetString();
}

}


bool ChordDatabaseParser::isValid(const std::string& jsonChord) const
{
    auto document = parse(jsonChord);
    if(document.HasParseError())
    {
        return false;
    }

    rapidjson::SchemaDocument schema(makeSchema());
    rapidjson::SchemaValidator validator(schema);
    return document.Accept(validator);
}

std::string ChordDatabaseParser::reportError(const std::string& jsonChord) const
{
    auto document = parse(jsonChord);
    if(document.HasParseError())
    {
        return rapidjson::GetParseError_En(document.GetParseError());
    }

    rapidjson::SchemaDocument schema(makeSchema());
    rapidjson::SchemaValidator validator(schema);
    if(not document.Accept(validator))
    {
        return getErrorAsString(validator);
    }

    throw std::runtime_error("Document is valid !");
}

}}}
