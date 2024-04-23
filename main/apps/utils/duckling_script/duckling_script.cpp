/**
 * @file duckling_script.cpp
 * @author basicx-StrgV
 * @brief
 * @version 0.1
 * @date 2024-04-23
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <list>
#include <string>
#include <mooncake.h>
#include "duckling_script.hpp"
#include "../../app_duckling/app_duckling.h"
#include "spdlog/spdlog.h"

using namespace DUCKLING::SCRIPT;

std::string trim(const std::string &source)
{
    std::string s(source);
    s.erase(0, s.find_first_not_of(" \n\r\t"));
    s.erase(s.find_last_not_of(" \n\r\t") + 1);
    return s;
}

void MOONCAKE::APPS::AppDuckling::_handle_payload(std::list<std::string> payload)
{
    bool in_rem_block = false;
    bool in_string_block = false;

    // Process payload line by line
    for (std::string const &i : payload)
    {
        // Get the timmrd down payload line
        std::string line = trim(i);

        // Handle comments ------------------------------------------------------------------
        if (line.rfind("END_REM") != std::string::npos)
        {
            // Exit rem block
            in_rem_block = false;

            // Skip to next line
            continue;
        }
        else if (in_rem_block)
        {
            // Skip in block comment
            continue;
        }
        else if (line.rfind("REM_BLOCK", 0) != std::string::npos)
        {
            // Start rem block
            in_rem_block = true;

            // Skip to next line
            continue;
        }
        else if (line.rfind("REM", 0) != std::string::npos)
        {
            // Skip comment
            continue;
        }

        // Handle strings -------------------------------------------------------------------
        if (line.rfind("STRING", 0) != std::string::npos && line.size() > 8)
        {
            // Write the string, after the keyword, plus one space inbetween
            _write_string(line.substr(7), false);
        }
        else if (line.rfind("STRINGLN", 0) != std::string::npos && line.size() > 10)
        {
            // Write the string, after the keyword, plus one space inbetween
            _write_string(line.substr(9), true);
        }
    }
}

void MOONCAKE::APPS::AppDuckling::_write_string(std::string line, bool add_line_brake)
{
    for (char const &i : line)
    {
        std::string key(1, i);

        DUCKLING::LANGUAGE::CodeInfo_t code_info = _data.lang.get_code_info(key);

        if (_data.kb_type == kb_type_ble)
        {
            _ble_kb_update_kb_input();
        }
        else if (_data.kb_type == kb_type_usb)
        {
            _usb_kb_update_kb_input(code_info.keycode, code_info.modifier_keys);
        }

        // ets_delay_us(100);
    }

    if (add_line_brake)
    {
        DUCKLING::LANGUAGE::CodeInfo_t code_info = _data.lang.get_code_info("ENTER");

        if (_data.kb_type == kb_type_ble)
        {
            _ble_kb_update_kb_input();
        }
        else if (_data.kb_type == kb_type_usb)
        {
            _usb_kb_update_kb_input(code_info.keycode, code_info.modifier_keys);
        }
    }
}