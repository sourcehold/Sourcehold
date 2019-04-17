#include <System/FileUtil.h>

#include <boost/filesystem.hpp>
#include <iostream>

using namespace Sourcehold;
using namespace System;

std::string System::GetFileExtension(boost::filesystem::path path) {
    return path.extension().string();
}

std::string System::GetFileStem(boost::filesystem::path path) {
    return path.stem().string();
}

std::string System::GetFilename(boost::filesystem::path path) {
    return path.filename().string();
}

bool System::IsFileHidden(boost::filesystem::path path) {
    std::string name = path.filename().string();
    if(name[0] == '.') return true;
    return false;
}

std::vector<boost::filesystem::path> System::GetDirectoryRecursive(boost::filesystem::path path, const std::string &extension, bool recursive){
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
