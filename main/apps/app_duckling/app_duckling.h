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
                int lines = 1;
                std::string text_buffer;
                uint32_t cursor_update_time_count = 0;
                uint32_t cursor_update_period = 500;
                bool cursor_state = false;
                bool waiting_user_input = false;
                bool can_skip = false;
                DialogAction_t dialog_action = DialogAction_t::none;
                // Keyboard
                uint32_t update_infos_time_count = 0;
                uint32_t update_kb_time_count = 0;
                KeyboardType_t kb_type = kb_type_ble;
            };
            Data_t _data;

            void _update_input();
            void _update_cursor();
            void _dialog(const std::string &message, bool can_skip);
            // Keyboard
            void _select_kb_type();
            /*void _ble_kb_init();
            void _ble_kb_update_infos();
            void _ble_kb_update_kb_input();
            void _usb_kb_init();
            void _usb_kb_update_infos();
            void _usb_kb_update_kb_input();*/

        public:
            void onCreate() override;
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