#pragma once

#include <string>
#include <vector>

namespace Sourcehold
{
    namespace System
    {
        /*
         * Get the extension of a single file
         */
        std::string GetFileExtension(const std::string &path);

        /*
         * Get all the files in a directory and its
         * sub directories
         */
        std::vector<std::string> GetDirectoryRecursive(const std::string &path, const std::string &extension = "", bool recursive = true);
    }
}
