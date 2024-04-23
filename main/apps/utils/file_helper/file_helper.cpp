/**
 * @file file_helper.cpp
 * @author basicx-StrgV
 * @brief
 * @version 0.1
 * @date 2024-04-23
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <string>
#include "file_helper.hpp"

using namespace FILES::HELPER;

std::string FileReader::get_file_content(std::string file)
{
    std::string fileContent;

    // Read file content
    FILE *f = fopen(file.c_str(), "r");
    if (f != NULL)
    {
        char text[100];
        while (fgets(text, 80, f))
        {
            fileContent += text;
        }
    }

    return fileContent;
}