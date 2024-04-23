/**
 * @file app_duckling.cpp
 * @author basicx-StrgV
 * @brief
 * @version 0.1
 * @date 2024-04-20
 *
 * @copyright Copyright (c) 2024
 *
 */
#include <list>
#include <string>
#include "app_duckling.h"
#include "spdlog/spdlog.h"
#include "../utils/theme/theme_define.h"

using namespace MOONCAKE::APPS;

#define _keyboard _data.hal->keyboard()
#define _canvas _data.hal->canvas()
#define _canvas_update _data.hal->canvas_update
#define _canvas_clear() _canvas->fillScreen(THEME_COLOR_BG)
#define _sdcard _data.hal->sdcard()

void AppDuckling::onCreate()
{
    spdlog::info("{} onCreate", getAppName());
    _data.hal = mcAppGetDatabase()->Get("HAL")->value<HAL::Hal *>();
}

void AppDuckling::onResume()
{
    spdlog::info("{} onResume", getAppName());
    ANIM_APP_OPEN();
    _data.text_buffer = "";
    _canvas_clear();
    _canvas->setTextScroll(true);
    _canvas->setBaseColor(THEME_COLOR_BG);
    _canvas->setTextColor(TFT_WHITE, THEME_COLOR_BG);
    _canvas->setFont(FONT_REPL);
    _canvas->setTextSize(FONT_SIZE_REPL);
    _canvas->setCursor(0, 0);

    if (!_sdcard->mount(false))
    {
        _dialog("SD card mount failed\nPress enter to try again\nctrl + q to quit\nctrl + f to format", false);
        _data.waiting_user_input = false;
    }
    else
    {
        spdlog::info("SD card mounted");

        _data.working_dir = _sdcard->get_filepath("duckling");

        // Check if workin dir exist
        if (!_sdcard->file_exists(_data.working_dir.c_str()))
        {
            spdlog::info("duckling folder not found, trying to create folder");

            // Try to create working dir
            if (_sdcard->create_dir(_data.working_dir.c_str()))
            {
                spdlog::info("duckling folder created succefully");
            }
            else
            {
                spdlog::info("failed to create duckling folder");

                _data.has_working_dir = false;
            }
        }

        // Init if working dir exists
        if (_data.has_working_dir)
        {
            std::list<std::string> file_list = _sdcard->get_dir_content(_data.working_dir.c_str());

            //  Get language definition files
            for (std::string const &i : file_list)
            {
                size_t extension_start_pos = i.find_last_of(".");

                if (extension_start_pos != std::string::npos)
                {
                    std::string file_type = i.substr(extension_start_pos + 1);

                    if (file_type == "json")
                    {
                        _data.lang_file_list.push_back(i);
                    }
                }
            }

            // Show lang selection; Cancel app if no selection was made
            if (!_select_kb_lang())
                return;

            spdlog::info("Language selected: {}", _data.kb_lang_file);

            // Load lang definition from selected file
            std::string langFilePath = (_data.working_dir + "/" + _data.kb_lang_file);

            std::string fileContent;

            // Read file content
            FILE *f = fopen(langFilePath.c_str(), "r");
            if (f != NULL)
            {
                char text[100];
                while (fgets(text, 80, f))
                {
                    fileContent += text;
                }
            }

            // Load language definition from file
            _data.lang.load(fileContent);

            // Show keyboard selection; Cancel app if no selection was made
            if (!_select_kb_type())
                return;

            if (_data.kb_type == kb_type_ble)
                _ble_kb_init();
            else if (_data.kb_type == kb_type_usb)
                _usb_kb_init();
        }
        else
        {
            _dialog("Working directory 'duckling' does not exist", false);
        }
    }
    _canvas_update();
}

void AppDuckling::onRunning()
{
    _update_input();

    if (_data.hal->homeButton()->pressed())
    {
        _data.hal->playNextSound();
        spdlog::info("quit duckling");

        destroyApp();
    }

    if (_data.has_working_dir)
    {

        if (_data.kb_type == kb_type_ble)
        {
            _ble_kb_update_infos();
            _ble_kb_update_kb_input();
        }
        else if (_data.kb_type == kb_type_usb)
        {
            _usb_kb_update_infos();
            _usb_kb_update_kb_input(0x00, 0x00);
        }
    }
}

void AppDuckling::_dialog(const std::string &message, bool can_skip)
{
    spdlog::info("dialog: {}", message);
    _canvas_clear();
    _canvas->setCursor(0, 0);
    _data.text_buffer = "";
    _canvas->setBaseColor(THEME_COLOR_BG);
    _canvas->setTextColor(TFT_RED, THEME_COLOR_BG);
    _canvas->print(message.c_str());
    _data.waiting_user_input = true;
    _data.can_skip = can_skip;
    _data.dialog_action = DialogAction_t::none;
    if (can_skip)
    {
        _canvas->print("\nPress enter to continue\n");
    }
    _canvas->setTextColor(TFT_WHITE, THEME_COLOR_BG);
    _canvas_update();
}

void AppDuckling::_update_input()
{
    // spdlog::info("{} {}", _keyboard->keyList().size(), _data.last_key_num);

    // If changed
    if (_keyboard->keyList().size() != _data.last_key_num)
    {
        // If key pressed
        if (_keyboard->keyList().size() != 0)
        {
            // Update states and values
            _keyboard->updateKeysState();

            if (_keyboard->keysState().enter)
            {
                if (!_sdcard->is_mounted())
                {
                    onResume();
                }
            }
            else if (_keyboard->keysState().ctrl)
            {
                _data.last_key_num = _keyboard->keyList().size();
                if (_keyboard->isKeyPressing(16))
                { // Q
                    spdlog::info("quit duckling");
                    destroyApp();
                }
                else if (_keyboard->isKeyPressing(34))
                { // F
                    spdlog::info("format sd card");
                    _dialog("Format SD card? [y/N]: ", false);
                    _data.dialog_action = DialogAction_t::format;
                }
                return;
            }

            _canvas_update();

            // Update last key num
            _data.last_key_num = _keyboard->keyList().size();
        }
        else
        {
            // Reset last key num
            _data.last_key_num = 0;
        }
    }
}

void AppDuckling::onDestroy()
{
    _canvas->setTextScroll(false);
    _sdcard->eject();

    delay(200);
    esp_restart();
    delay(10000);
}