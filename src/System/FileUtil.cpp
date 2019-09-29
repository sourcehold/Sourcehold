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

#include <boost/filesystem.hpp>

using namespace Sourcehold;
using namespace System;

std::string System::GetFileExtension(boost::filesystem::path path)
{
    return path.extension().string();
}

std::string System::GetFileStem(boost::filesystem::path path)
{
    return path.stem().string();
}

std::string System::GetFilename(boost::filesystem::path path)
{
    return path.filename().string();
}

bool System::IsFileHidden(boost::filesystem::path path)
{
    std::string name = path.filename().string();
    if(name[0] == '.') return true;
    return false;
}

bool System::DoesFileExist(boost::filesystem::path path)
{
    return boost::filesystem::exists(path);
}

void System::CreateFolder(boost::filesystem::path path)
{
    boost::filesystem::create_directories(path);
}

boost::filesystem::path System::GetDocumentsPath()
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

    return boost::filesystem::path(path);
}

std::vector<boost::filesystem::path> System::GetDirectoryRecursive(boost::filesystem::path path, const std::string &extension, bool recursive)
{
    std::vector<boost::filesystem::path> files;
    if(boost::filesystem::exists(path) && boost::filesystem::is_directory(path)) {
        if(recursive) {
            boost::filesystem::recursive_directory_iterator it(path);
            boost::filesystem::recursive_directory_iterator endit;
            while (it != endit) {
                if((extension=="")?true:it->path().extension() == extension) {
                    if(boost::filesystem::is_regular_file(*it)) files.push_back(it->path());
                }
                ++it;
            }
        }
        else {
            boost::filesystem::directory_iterator it(path);
            boost::filesystem::directory_iterator endit;
            while (it != endit) {
                if((extension=="")?true:it->path().extension() == extension) {
                    if(boost::filesystem::is_regular_file(*it)) files.push_back(it->path());
                }
                ++it;
            }
        }
    }
    return files;
}
