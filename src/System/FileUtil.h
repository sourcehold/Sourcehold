#pragma once

#include <string>
#include <vector>

#include <filesystem>

namespace Sourcehold {
    namespace System {
        std::string GetFileExtension(std::filesystem::path path);
        std::string GetFileStem(std::filesystem::path path);
        std::string GetFilename(std::filesystem::path path);

        bool IsFileHidden(std::filesystem::path path);
        bool IsFolder(std::filesystem::path path);
        bool DoesFileExist(std::filesystem::path path);
        void CreateFolder(std::filesystem::path path);

        std::filesystem::path GetDocumentsPath();

        /**
         * Get all the files in a directory and its
         * sub directories
         */
        std::vector<std::filesystem::path> GetDirectoryRecursive(std::filesystem::path path, const std::string &extension = "", bool recursive = true, bool includeFolders = false);
    }
}
