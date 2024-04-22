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

#pragma once
#include <map>
#include <string>

namespace DUCKLING
{
    namespace LANGUAGE
    {
        struct CodeInfo_t
        {
            uint8_t modifier_keys = 0x00;
            uint8_t reserved = 0x00;
            uint8_t keycode = 0x00;
        };

        class LangDefinition
        {
        private:
            std::map<std::string, CodeInfo_t> _keymap;

        public:
            void load(std::string jsonContent);
            CodeInfo_t get_code_info(std::string identifier);
        };
    }
}