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

#include <algorithm>

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
void addMember(rapidjson::Value& parent,
               rapidjson::Value& child,
               const std::string& name,
               Allocator& allocator)
{
    rapidjson::Value member(rapidjson::kStringType);
    member.SetString(name.c_str(), name.length(), allocator);

    parent.AddMember(member, child, allocator);
}

template<typename Allocator>
void addSecondLevelProperties(rapidjson::Value& properties,
                              rapidjson::Value& required,
                              Allocator& allocator)
{
    rapidjson::Value frets(rapidjson::kObjectType);
    addStringMember(frets, "type", "string", allocator);
    addMember(properties, frets, "frets", allocator);
    required.PushBack(rapidjson::Value("frets").Move(), allocator);

    rapidjson::Value fingers(rapidjson::kObjectType);
    addStringMember(fingers, "type", "string", allocator);
    addMember(properties, fingers, "fingers", allocator);
    required.PushBack(rapidjson::Value("fingers").Move(), allocator);

    rapidjson::Value barres(rapidjson::kObjectType);
    addStringMember(barres, "type", "integer", allocator);
    addMember(properties, barres, "barres", allocator);

    rapidjson::Value capo(rapidjson::kObjectType);
    addStringMember(capo, "type", "boolean", allocator);
    addMember(properties, capo, "capo", allocator);
}

template<typename Allocator>
void fillPositions(rapidjson::Value& positions, Allocator& allocator)
{
    rapidjson::Value items(rapidjson::kObjectType);
    addStringMember(items, "type", "object", allocator);

    rapidjson::Value propertiesOfItems(rapidjson::kObjectType);
    rapidjson::Value requiredOfItems(rapidjson::kArrayType);

    addSecondLevelProperties(propertiesOfItems, requiredOfItems, allocator);

    addMember(items, requiredOfItems, "required", allocator);
    addMember(items, propertiesOfItems, "properties", allocator);
    addMember(positions, items, "items", allocator);
    addStringMember(positions, "type", "array", allocator);

    rapidjson::Value minItems(rapidjson::kNumberType);
    minItems.SetInt(1);
    addMember(positions, minItems, "minItems", allocator);
}

template<typename Allocator>
void addFirstLevelProperties(rapidjson::Value& properties,
                             rapidjson::Value& required,
                             Allocator& allocator)
{
    rapidjson::Value key(rapidjson::kObjectType);
    addStringMember(key, "type", "string", allocator);
    addMember(properties, key, "key", allocator);
    required.PushBack(rapidjson::Value("key").Move(), allocator);

    rapidjson::Value suffix(rapidjson::kObjectType);
    addStringMember(suffix, "type", "string", allocator);
    addMember(properties, suffix, "suffix", allocator);
    required.PushBack(rapidjson::Value("suffix").Move(), allocator);

    rapidjson::Value positions(rapidjson::kObjectType);
    fillPositions(positions, allocator);
    addMember(properties, positions, "positions", allocator);
    required.PushBack(rapidjson::Value("positions").Move(), allocator);
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
    rapidjson::Value required(rapidjson::kArrayType);

    addFirstLevelProperties(properties, required, allocator);

    addMember(document, required, "required", allocator);
    addMember(document, properties, "properties", allocator);

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

Position buildPosition(const rapidjson::Value& position)
{
    return Position
    {
        position["fingers"].GetString(),
        position["frets"].GetString(),
        position.HasMember("barres")
            ? position["barres"].GetUint64()
            : 0U,
        position.HasMember("capo")
            ? position["capo"].GetBool()
            : false,
    };
}

std::vector<Position> buildPositions(const rapidjson::Value& positions)
{
    std::vector<Position> result;
    result.reserve(positions.Size());

    std::transform(positions.Begin(),
                   positions.End(),
                   std::back_inserter(result),
                   &buildPosition);

    return result;
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

Chord ChordDatabaseParser::build(const std::string& jsonChord) const
{
    auto document = parse(jsonChord);
    return Chord
    {
        buildPositions(document["positions"]),
        document["suffix"].GetString(),
        document["key"].GetString()
    };
}

}}}
