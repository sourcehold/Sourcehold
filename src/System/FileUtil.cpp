#include "System/FileUtil.h"
#include "System/Config.h"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>

#if SOURCEHOLD_WINDOWS || SOURCEHOLD_MINGW
#include <windows.h>
#include <shlobj.h>
#endif

#include "System/filesystem.h"

#if SOURCEHOLD_MAC_OS || SOURCEHOLD_IOS
#include "../apple/Common/SHPathUtils.h"
#endif // SOURCEHOLD_MAC_OS || SOURCEHOLD_IOS


using namespace Sourcehold;
using namespace System;

std::string System::GetFileExtension(ghc::filesystem::path path)
{
    return path.extension().string();
}

std::string System::GetFileStem(ghc::filesystem::path path)
{
    return path.stem().string();
}

std::string System::GetFilename(ghc::filesystem::path path)
{
    return path.filename().string();
}

bool System::IsFileHidden(ghc::filesystem::path path)
{
    std::string name = path.filename().string();
    if(name[0] == '.') return true;
    return false;
}

bool System::IsFolder(ghc::filesystem::path path)
{
    return ghc::filesystem::is_directory(path);
}

bool System::DoesFileExist(ghc::filesystem::path path)
{
    return ghc::filesystem::exists(path);
}

void System::CreateFolder(ghc::filesystem::path path)
{
    ghc::filesystem::create_directories(path);
}

ghc::filesystem::path System::GetDocumentsPath()
{
    static const unsigned int maxPathLength = 512;
    char path[maxPathLength];

    /**
    * TODO: better way to do this
    * Why the f*** isn't this part of boost?
    */
#if SOURCEHOLD_MAC_OS || SOURCEHOLD_IOS
    SHGetUserDocumentsDirectoryPath(path, maxPathLength);
#elif SOURCEHOLD_UNIX
    /* Requires ~/.config/user-dirs.dirs from the FreeDesktop xdg standard */
    const char *home = getenv("HOME");
    strcpy(path, home);
    strcat(path, "/.config/user-dirs.dirs");

    if(DoesFileExist(path)) {
        /* Based on https://github.com/sago007/PlatformFolders/blob/master/sago/platform_folders.cpp#L262 */
        std::ifstream infile(path);
        std::string line;
        while (std::getline(infile, line)) {
            if (line.length() == 0 || line.at(0) == '#' || line.substr(0, 4) != "XDG_" || line.find("_DIR") == std::string::npos) {
                continue;
            }

            std::size_t splitPos = line.find('=');
            std::string name = line.substr(0, splitPos);
            std::size_t valueStart = line.find('"', splitPos);
            std::size_t valueEnd = line.find('"', valueStart+1);
            std::string value = line.substr(valueStart+1, valueEnd - valueStart - 1);

            if (value.compare(0, 5, "$HOME") == 0) {
                value = std::string(home) + value.substr(5, std::string::npos);
            }

            if(name == "XDG_DOCUMENTS_DIR") {
                strcpy(path, value.c_str());
            }
        }
        infile.close();
    }
    else {
        *path = '\0';
    }
#elif SOURCEHOLD_WINDOWS || SOURCEHOLD_MINGW
    LPITEMIDLIST pidl;
    SHGetSpecialFolderLocation(NULL, CSIDL_PERSONAL, &pidl);
    SHGetPathFromIDList(pidl, path);
#endif // SOURCEHOLD_MAC_OS || SOURCEHOLD_IOS

    return ghc::filesystem::path(path);
}

std::vector<ghc::filesystem::path> System::GetDirectoryRecursive(ghc::filesystem::path path, const std::string &extension, bool recursive, bool includeFolders)
{
    std::vector<ghc::filesystem::path> files;
    if(ghc::filesystem::exists(path) && ghc::filesystem::is_directory(path)) {
        if(recursive) {
            ghc::filesystem::recursive_directory_iterator it(path);
            ghc::filesystem::recursive_directory_iterator endit;
            while (it != endit) {
                if((extension=="")?true:it->path().extension() == extension) {
                    if(ghc::filesystem::is_regular_file(*it) || includeFolders) files.push_back(it->path());
                }
                ++it;
            }
        }
        else {
            ghc::filesystem::directory_iterator it(path);
            ghc::filesystem::directory_iterator endit;
            while (it != endit) {
                if((extension=="")?true:it->path().extension() == extension) {
                    if(ghc::filesystem::is_regular_file(*it) || includeFolders) files.push_back(it->path());
                }
                ++it;
            }
        }
    }
    return files;
}
