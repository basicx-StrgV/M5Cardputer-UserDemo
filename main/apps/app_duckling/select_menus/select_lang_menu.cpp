/**
 * @file select_lang_menu.cpp
 * @author basicx-StrgV
 * @brief
 * @version 0.1
 * @date 2024-04-21
 *
 * @copyright Copyright (c) 2024
 *
 */
#include <list>
#include <string>
#include "../app_duckling.h"
#include "spdlog/spdlog.h"
#include "../../utils/theme/theme_define.h"

using namespace MOONCAKE::APPS;

struct SelectLang_t
{
    std::string tag;
    int x = 0;
    int y = 0;
    SelectLang_t(std::string tag, int x, int y)
    {
        this->tag = tag;
        this->x = x;
        this->y = y;
    }
};

bool AppDuckling::_select_kb_lang()
{
    int8_t selected = 0;
    bool is_select_change = true;
    std::vector<SelectLang_t> item_list;

    int entryCounter = 1;
    for (std::string const &i : _data.lang_file_list)
    {
        // Place item on an interval of "17"
        item_list.push_back(SelectLang_t(i, 0, (17 * entryCounter)));

        entryCounter++;
    }

    bool showSelection = true;

    while (showSelection)
    {
        // Update select
        _data.hal->keyboard()->updateKeyList();
        _data.hal->keyboard()->updateKeysState();

        if (_data.hal->homeButton()->pressed())
        {
            _data.hal->playNextSound();
            spdlog::info("quit duckling");

            showSelection = false;

            destroyApp();
        }

        if (_data.hal->keyboard()->isKeyPressing(55) || _data.hal->keyboard()->isKeyPressing(54))
        {
            _data.hal->playKeyboardSound();

            // Hold till release
            while (_data.hal->keyboard()->isKeyPressing(55) || _data.hal->keyboard()->isKeyPressing(54))
                _data.hal->keyboard()->updateKeyList();

            selected++;
            if (selected >= item_list.size())
                selected = 0;
            is_select_change = true;

            _data.hal->playNextSound();
        }
        else if (_data.hal->keyboard()->isKeyPressing(53) || _data.hal->keyboard()->isKeyPressing(40))
        {
            _data.hal->playKeyboardSound();

            // Hold till release
            while (_data.hal->keyboard()->isKeyPressing(53) || _data.hal->keyboard()->isKeyPressing(40))
                _data.hal->keyboard()->updateKeyList();

            selected--;
            if (selected < 0)
                selected = item_list.size() - 1;
            is_select_change = true;

            _data.hal->playNextSound();
        }
        else if (_data.hal->keyboard()->isKeyPressing(42))
        {
            _data.hal->playKeyboardSound();

            // Hold till release
            while (_data.hal->keyboard()->isKeyPressing(42))
                _data.hal->keyboard()->updateKeyList();

            _data.hal->playNextSound();

            // Quit
            break;
        }

        // Render
        if (is_select_change)
        {
            is_select_change = false;

            _data.hal->canvas()->fillScreen(THEME_COLOR_BG);
            _data.hal->canvas()->setTextColor(TFT_ORANGE, THEME_COLOR_BG);
            _data.hal->canvas()->setCursor(0, 0);
            _data.hal->canvas()->print("[Select Language]");

            for (int i = 0; i < item_list.size(); i++)
            {
                if (i == selected)
                {
                    _data.hal->canvas()->fillSmoothCircle(item_list[i].x + 12, item_list[i].y + 8, 6, TFT_GREENYELLOW);
                    _data.hal->canvas()->setTextColor(TFT_GREENYELLOW, THEME_COLOR_BG);
                }
                else
                    _data.hal->canvas()->setTextColor(TFT_ORANGE, THEME_COLOR_BG);

                _data.hal->canvas()->setCursor(item_list[i].x + 26, item_list[i].y);
                _data.hal->canvas()->print(item_list[i].tag.c_str());
            }

            _data.hal->canvas_update();
        }
    }

    spdlog::info("selected {}", item_list[selected].tag);
    _data.kb_lang_file = item_list[selected].tag;

    // Return if a selection was made; If false is returned the selection was canceled
    return showSelection;
}
