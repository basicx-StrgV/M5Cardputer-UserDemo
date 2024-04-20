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
    _data.lines = 1;
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

        _select_kb_type();
    }
    _canvas_update();
}

void AppDuckling::onRunning()
{
    _update_input();
    _update_cursor();

    if (_data.hal->homeButton()->pressed())
    {
        _data.hal->playNextSound();
        spdlog::info("quit duckling");

        destroyApp();
    }

    /*if (_data.kb_type == kb_type_ble)
    {
        _ble_kb_update_infos();
        _ble_kb_update_kb_input();
    }
    else if (_data.kb_type == kb_type_usb)
    {
        _usb_kb_update_infos();
        _usb_kb_update_kb_input();
    }*/
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

void AppDuckling::_update_cursor()
{
    if ((millis() - _data.cursor_update_time_count) > _data.cursor_update_period)
    {
        // Fix cursor position
        if (_canvas->getCursorX() == 0 || _canvas->width() - _canvas->getCursorX() < FONT_REPL_WIDTH)
        {
            _canvas->print(" ");
            _canvas->setCursor(0, _canvas->getCursorY());
        }

        // Get cursor
        int cursor_x = _canvas->getCursorX();
        int cursor_y = _canvas->getCursorY();

        _canvas->print(_data.cursor_state ? '_' : ' ');
        _canvas->setCursor(cursor_x, cursor_y);
        _canvas_update();

        _data.cursor_state = !_data.cursor_state;
        _data.cursor_update_time_count = millis();
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