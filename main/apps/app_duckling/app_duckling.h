/**
 * @file app_duckling.h
 * @author basicx-StrgV
 * @brief
 * @version 0.1
 * @date 2024-04-20
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once
#include <mooncake.h>
#include "../../hal/hal.h"
#include "../utils/theme/theme_define.h"
#include "../utils/anim/anim_define.h"
#include "../utils/icon/icon_define.h"
#include "../utils/lang_definition/lang_definition.hpp"

#include "assets/duckling_big.h"
#include "assets/duckling_small.h"

namespace MOONCAKE
{
    namespace APPS
    {
        class AppDuckling : public APP_BASE
        {
        private:
            enum DialogAction_t
            {
                none = 0,
                format
            };
            enum KeyboardType_t
            {
                kb_type_ble = 0,
                kb_type_usb,
            };
            struct Data_t
            {
                HAL::Hal *hal = nullptr;
                int last_key_num = 0;
                std::string text_buffer;
                bool waiting_user_input = false;
                bool can_skip = false;
                DialogAction_t dialog_action = DialogAction_t::none;
                // Working dir
                bool has_working_dir = true;
                std::string working_dir;
                std::list<std::string> lang_file_list;
                std::list<std::string> payload_file_list;
                // Language
                std::string kb_lang_file = "";
                DUCKLING::LANGUAGE::LangDefinition lang;
                // Keyboard
                uint32_t update_infos_time_count = 0;
                uint32_t update_kb_time_count = 0;
                KeyboardType_t kb_type = kb_type_ble;
            };
            Data_t _data;

            void _update_input();
            void _dialog(const std::string &message, bool can_skip);
            // Language
            bool _select_kb_lang();
            // Keyboard
            bool _select_kb_type();
            void _ble_kb_init();
            void _ble_kb_update_infos();
            void _ble_kb_update_kb_input();
            // bool _ble_kb_mounted();
            void _usb_kb_init();
            void _usb_kb_update_infos();
            void _usb_kb_update_kb_input(uint8_t key, uint8_t modifier = 0x00);
            bool _usb_kb_mounted();
            // Payload
            std::string _select_payload();
            void _handle_payload(std::list<std::string> payload);
            void _write_string(std::string line, bool add_line_brake);

        public:
            void
            onCreate() override;
            void onResume() override;
            void onRunning() override;
            void onDestroy() override;
        };

        class AppDuckling_Packer : public APP_PACKER_BASE
        {
            std::string getAppName() override { return "DUCKLING"; }
            void *getAppIcon() override { return (void *)(new AppIcon_t(image_data_duckling_big, image_data_duckling_small)); }
            void *newApp() override { return new AppDuckling; }
            void deleteApp(void *app) override { delete (AppDuckling *)app; }
        };
    }
}