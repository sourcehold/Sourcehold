#include <System/FileUtil.h>

#include <boost/filesystem.hpp>
#include <iostream>

using namespace Sourcehold;
using namespace System;

std::string System::GetFileExtension(const std::string &path) {
    boost::filesystem::path p(path);
    return p.extension().string();
}

std::string System::GetFileStem(const std::string &path) {
    boost::filesystem::path p(path);
    return p.stem().string();
}

std::string System::GetFilename(const std::string &path) {
    boost::filesystem::path p(path);
    return p.filename().string();
}

bool System::IsFileHidden(const std::string &path) {
    std::string name = boost::filesystem::path(path).filename().string();
    if(name[0] == '.') return true;
    return false;
}

std::vector<std::string> System::GetDirectoryRecursive(const std::string &path, const std::string &extension, bool recursive){
    std::vector<std::string> files;
    boost::filesystem::path dir(path);
    if(boost::filesystem::exists(path) && boost::filesystem::is_directory(path)) {
        if(recursive) {
            boost::filesystem::recursive_directory_iterator it(path);
            boost::filesystem::recursive_directory_iterator endit;
            while (it != endit) {
                if((extension=="")?true:it->path().extension() == extension) {
                    if(boost::filesystem::is_regular_file(*it)) files.push_back(it->path().string());
                }
                ++it;
            }
        }
        else {
            boost::filesystem::directory_iterator it(path);
            boost::filesystem::directory_iterator endit;
            while (it != endit) {
                if((extension=="")?true:it->path().extension() == extension) {
                    if(boost::filesystem::is_regular_file(*it)) files.push_back(it->path().string());
                }
                ++it;
            }
        }
    }
    return files;
}
