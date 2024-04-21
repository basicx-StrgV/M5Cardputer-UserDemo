/**
 * @file usb_keyboard.cpp
 * @author Forairaaaaa, basicx-StrgV
 * @brief
 * @version 0.1
 * @date 2024-04-20
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "../../app_keyboard/app_keyboard.h"
#include "../../app_duckling/app_duckling.h"
#include "spdlog/spdlog.h"
#include "../theme/theme_define.h"
#include "usb_keyboard_wrap.h"

using namespace MOONCAKE::APPS;

enum UsbKeyboardState_t
{
    _state_wait_connect = 0,
    _state_mounted,
};
static UsbKeyboardState_t _current_state = _state_wait_connect;
static bool _is_state_updated = true;

// Invoked when device is mounted
extern "C" void tud_mount_cb(void)
{
    _current_state = _state_mounted;
    _is_state_updated = true;
}

// Invoked when device is unmounted
extern "C" void tud_umount_cb(void)
{
    _current_state = _state_wait_connect;
    _is_state_updated = true;
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
extern "C" void tud_suspend_cb(bool remote_wakeup_en)
{
    (void)remote_wakeup_en;
}

// Invoked when usb bus is resumed
extern "C" void tud_resume_cb(void)
{
}

void AppKeyboard::_usb_kb_init()
{
    usb_kb_wrap_init();
}

void AppDuckling::_usb_kb_init()
{
    usb_kb_wrap_init();
}

void AppKeyboard::_usb_kb_update_infos()
{
    if (_is_state_updated)
    {
        _is_state_updated = false;

        _data.hal->canvas()->fillScreen(THEME_COLOR_BG);

        _data.hal->canvas()->setCursor(0, 0);
        _data.hal->canvas()->setTextColor(TFT_ORANGE, THEME_COLOR_BG);
        _data.hal->canvas()->printf("[USB Keyboard]\n");
        // _data.hal->canvas()->printf("> Name: %s\n", ble_keyboard_wrap_get_device_name());

        // State
        _data.hal->canvas()->printf("> State: ");
        _data.hal->canvas()->setTextColor(
            _current_state == _state_mounted ? TFT_GREENYELLOW : TFT_ORANGE,
            THEME_COLOR_BG);
        _data.hal->canvas()->printf(
            "%s\n",
            _current_state == _state_mounted ? "Mounted" : "Wait connect..");

        _data.hal->canvas_update();
    }
}

void AppDuckling::_usb_kb_update_infos()
{
    if (_is_state_updated)
    {
        _is_state_updated = false;

        _data.hal->canvas()->fillScreen(THEME_COLOR_BG);

        _data.hal->canvas()->setCursor(0, 0);
        _data.hal->canvas()->setTextColor(TFT_ORANGE, THEME_COLOR_BG);
        _data.hal->canvas()->printf("[USB Keyboard]\n");
        // _data.hal->canvas()->printf("> Name: %s\n", ble_keyboard_wrap_get_device_name());

        // State
        _data.hal->canvas()->printf("> State: ");
        _data.hal->canvas()->setTextColor(
            _current_state == _state_mounted ? TFT_GREENYELLOW : TFT_ORANGE,
            THEME_COLOR_BG);
        _data.hal->canvas()->printf(
            "%s\n",
            _current_state == _state_mounted ? "Mounted" : "Wait connect..");

        _data.hal->canvas_update();
    }
}

static uint8_t _input_buffer[6] = {0, 0, 0, 0, 0, 0};

void AppKeyboard::_usb_kb_update_kb_input()
{
    if (_current_state != _state_mounted)
        return;

    if (millis() - _data.update_kb_time_count > 10)
    {
        if (_data.hal->keyboard()->isChanged())
        {
            uint8_t modifier = 0;
            if (_data.hal->keyboard()->isPressed())
            {
                memset(_input_buffer, 0, 6);
                auto status = _data.hal->keyboard()->keysState();

                int count = 0;
                for (auto &i : status.hidKey)
                {
                    if (count < 5)
                    {
                        _input_buffer[1 + count] = i;
                        count++;
                    }
                }

                if (status.ctrl)
                {
                    // ESP_LOGI(TAG, "CONTROL");
                    // modifier |= 0x01;
                    modifier |= 0xE0;
                }

                if (status.shift || _data.hal->keyboard()->capslocked())
                {
                    // ESP_LOGI(TAG, "SHIFT");
                    // modifier |= 0x02;
                    modifier |= 0xE1;
                }

                if (status.alt)
                {
                    // ESP_LOGI(TAG, "ALT");
                    // modifier |= 0x03;
                    modifier |= 0xE2;
                }

                _input_buffer[0] = modifier;

                // ESP_LOG_BUFFER_HEX(TAG, buffer, 8);
                usb_kb_wrap_report(_input_buffer);
            }
            else
            {
                memset(_input_buffer, 0, 6);
                usb_kb_wrap_report(_input_buffer);
            }
        }

        _data.update_kb_time_count = millis();
    }
}

void AppDuckling::_usb_kb_update_kb_input()
{
    if (_current_state != _state_mounted)
        return;

    if (millis() - _data.update_kb_time_count > 10)
    {
        if (_data.hal->keyboard()->isChanged())
        {
            uint8_t modifier = 0;
            if (_data.hal->keyboard()->isPressed())
            {
                memset(_input_buffer, 0, 6);
                auto status = _data.hal->keyboard()->keysState();

                int count = 0;
                for (auto &i : status.hidKey)
                {
                    if (count < 5)
                    {
                        _input_buffer[1 + count] = i;
                        count++;
                    }
                }

                if (status.ctrl)
                {
                    // ESP_LOGI(TAG, "CONTROL");
                    // modifier |= 0x01;
                    modifier |= 0xE0;
                }

                if (status.shift || _data.hal->keyboard()->capslocked())
                {
                    // ESP_LOGI(TAG, "SHIFT");
                    // modifier |= 0x02;
                    modifier |= 0xE1;
                }

                if (status.alt)
                {
                    // ESP_LOGI(TAG, "ALT");
                    // modifier |= 0x03;
                    modifier |= 0xE2;
                }

                _input_buffer[0] = modifier;

                // ESP_LOG_BUFFER_HEX(TAG, buffer, 8);
                usb_kb_wrap_report(_input_buffer);
            }
            else
            {
                memset(_input_buffer, 0, 6);
                usb_kb_wrap_report(_input_buffer);
            }
        }

        _data.update_kb_time_count = millis();
    }
}
