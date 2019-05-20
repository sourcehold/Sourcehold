#pragma once

#include <string>
#include <vector>

#include <boost/filesystem.hpp>

namespace Sourcehold
{
    namespace System
    {
        /**
         * Get the extension of a single file
         */
        std::string GetFileExtension(boost::filesystem::path path);

        /**
         * Get the stem of a single file
         */
        std::string GetFileStem(boost::filesystem::path path);

        /**
         * Get the filename from a path
         */
        std::string GetFilename(boost::filesystem::path path);

        bool IsFileHidden(boost::filesystem::path path);
		bool DoesFileExist(boost::filesystem::path path);

        /**
         * Get all the files in a directory and its
         * sub directories
         */
        std::vector<boost::filesystem::path> GetDirectoryRecursive(boost::filesystem::path path, const std::string &extension = "", bool recursive = true);
    }
}
