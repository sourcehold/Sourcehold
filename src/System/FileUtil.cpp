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

#include <filesystem>

using namespace Sourcehold;
using namespace System;

std::string System::GetFileExtension(std::filesystem::path path)
{
    return path.extension().string();
}

std::string System::GetFileStem(std::filesystem::path path)
{
    return path.stem().string();
}

std::string System::GetFilename(std::filesystem::path path)
{
    return path.filename().string();
}

bool System::IsFileHidden(std::filesystem::path path)
{
    std::string name = path.filename().string();
    if(name[0] == '.') return true;
    return false;
}

bool System::IsFolder(std::filesystem::path path)
{
    return std::filesystem::is_directory(path);
}

bool System::DoesFileExist(std::filesystem::path path)
{
    return std::filesystem::exists(path);
}

void System::CreateFolder(std::filesystem::path path)
{
    std::filesystem::create_directories(path);
}

std::filesystem::path System::GetDocumentsPath()
{
    char path[512];

    /**
    * TODO: better way to do this
    * Why the f*** isn't this part of boost?
    */
#ifdef SOURCEHOLD_UNIX
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
#endif

    return std::filesystem::path(path);
}

std::vector<std::filesystem::path> System::GetDirectoryRecursive(std::filesystem::path path, const std::string &extension, bool recursive, bool includeFolders)
{
    std::vector<std::filesystem::path> files;
    if(std::filesystem::exists(path) && std::filesystem::is_directory(path)) {
        if(recursive) {
            std::filesystem::recursive_directory_iterator it(path);
            std::filesystem::recursive_directory_iterator endit;
            while (it != endit) {
                if((extension=="")?true:it->path().extension() == extension) {
                    if(std::filesystem::is_regular_file(*it) || includeFolders) files.push_back(it->path());
                }
                ++it;
            }
        }
        else {
            std::filesystem::directory_iterator it(path);
            std::filesystem::directory_iterator endit;
            while (it != endit) {
                if((extension=="")?true:it->path().extension() == extension) {
                    if(std::filesystem::is_regular_file(*it) || includeFolders) files.push_back(it->path());
                }
                ++it;
            }
        }
    }
    return files;
}
