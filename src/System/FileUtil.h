#pragma once

#include <string>
#include <vector>

#include <boost/filesystem.hpp>

namespace Sourcehold
{
    namespace System
    {
        std::string GetFileExtension(boost::filesystem::path path);
        std::string GetFileStem(boost::filesystem::path path);
        std::string GetFilename(boost::filesystem::path path);

        bool IsFileHidden(boost::filesystem::path path);
		bool DoesFileExist(boost::filesystem::path path);
        void CreateFolder(boost::filesystem::path path);

        boost::filesystem::path GetDocumentsPath();

        /**
         * Get all the files in a directory and its
         * sub directories
         */
        std::vector<boost::filesystem::path> GetDirectoryRecursive(boost::filesystem::path path, const std::string &extension = "", bool recursive = true);
    }
}
