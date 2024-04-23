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

#include <list>
#include <string>
#include <limits.h>
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

std::list<std::string> FileReader::get_file_content_lines(std::string file)
{
    std::list<std::string> fileContent;

    // Read file content
    FILE *f = fopen(file.c_str(), "r");
    if (f != NULL)
    {
        char text[100];
        while (fgets(text, INT_MAX, f))
        {
            fileContent.push_back(text);
        }
    }

    return fileContent;
}