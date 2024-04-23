/**
 * @file file_helper.hpp
 * @author basicx-StrgV
 * @brief
 * @version 0.1
 * @date 2024-04-23
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include <list>
#include <string>

namespace FILES
{
    namespace HELPER
    {
        class FileReader
        {
        private:
        public:
            static std::string get_file_content(std::string file);
            static std::list<std::string> get_file_content_lines(std::string file);
        };
    }
}