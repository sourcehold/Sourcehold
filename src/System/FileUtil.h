#pragma once

#include <string>
#include <vector>

#include "System/filesystem.h"

namespace Sourcehold {
    namespace System {
        std::string GetFileExtension(ghc::filesystem::path path);
        std::string GetFileStem(ghc::filesystem::path path);
        std::string GetFilename(ghc::filesystem::path path);

        bool IsFileHidden(ghc::filesystem::path path);
        bool IsFolder(ghc::filesystem::path path);
        bool DoesFileExist(ghc::filesystem::path path);
        void CreateFolder(ghc::filesystem::path path);

        ghc::filesystem::path GetDocumentsPath();

        /**
         * Get all the files in a directory and its
         * sub directories
         */
        std::vector<ghc::filesystem::path> GetDirectoryRecursive(ghc::filesystem::path path, const std::string &extension = "", bool recursive = true, bool includeFolders = false);
    }
}
