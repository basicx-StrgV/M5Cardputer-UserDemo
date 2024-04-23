/**
 * @file lang_definition.cpp
 * @author basicx-StrgV
 * @brief
 * @version 0.1
 * @date 2024-04-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <map>
#include <string>
#include "../json/json.hpp"
#include "lang_definition.hpp"

using namespace DUCKLING::LANGUAGE;
using json = nlohmann::json;

void LangDefinition::load(std::string jsonContent)
{
    json jsonObject = json::parse(jsonContent);

    for (auto &el : jsonObject.items())
    {
        std::string identifier = el.key();
        std::string value_string = el.value();

        if (identifier == "__comment" || value_string.size() < 8)
        {
            // Skip comments and value strings with invalid length
            continue;
        }

        try
        {
            // Get hex values
            uint8_t modifier_keys_string = std::stoi(value_string.substr(0, 2), 0, 16);
            uint8_t reserved = std::stoi(value_string.substr(3, 2), 0, 16);
            uint8_t keycode = std::stoi(value_string.substr(6, 2), 0, 16);

            CodeInfo_t codeinfo;
            codeinfo.modifier_keys = modifier_keys_string;
            codeinfo.reserved = reserved;
            codeinfo.keycode = keycode;

            _keymap[identifier] = codeinfo;
        }
        catch (...)
        {
            // Skip entry on error
            continue;
        }
    }
}

CodeInfo_t LangDefinition::get_code_info(std::string identifier)
{
    if (!_keymap.count(identifier))
    {
        CodeInfo_t emptyData;

        return emptyData;
    }

    return _keymap[identifier];
}