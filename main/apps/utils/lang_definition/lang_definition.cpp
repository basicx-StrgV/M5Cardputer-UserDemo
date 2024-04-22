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
#include "spdlog/spdlog.h"

using namespace DUCKLING::LANGUAGE;
using json = nlohmann::json;

void LangDefinition::load(std::string jsonContent)
{
    json jsonObject = json::parse(jsonContent);

    // TODO
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