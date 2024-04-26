/**
 * @file duckling_script.hpp
 * @author basicx-StrgV
 * @brief
 * @version 0.1
 * @date 2024-04-23
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include <string>

namespace DUCKLING
{
    namespace SCRIPT
    {
        static std::string system_keys[19] = {
            "ENTER", "ESCAPE", "PAUSE", "BREAK", "PRINTSCREEN", "MENU", "APP", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12"};
        static std::string modifier_keys[7] = {
            "SHIFT", "ALT", "CONTROL", "CTRL", "COMMAND", "WINDOWS", "GUI"};
        static std::string lock_keys[3] = {
            "CAPSLOCK", "NUMLOCK", "SCROLLOCK"};
    }
}