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

std::vector<std::string> System::GetDirectoryRecursive(const std::string &path, bool recursive){
    std::vector<std::string> files;
    boost::filesystem::path dir(path);
    if(boost::filesystem::exists(path) && boost::filesystem::is_directory(dir)) {
        if(recursive) {
            boost::filesystem::recursive_directory_iterator it(dir);
            boost::filesystem::recursive_directory_iterator endit;
            while (it != endit) {
                if(boost::filesystem::is_regular_file(*it) && !IsFileHidden(it->path().string())) {
                    files.push_back(it->path().string());
                }
                ++it;
            }
        }
        else {
            boost::filesystem::directory_iterator it(dir);
            boost::filesystem::directory_iterator endit;
            while (it != endit) {
                if(boost::filesystem::is_regular_file(*it) && !IsFileHidden(it->path().string())) {
                    files.push_back(it->path().string());
                }
                ++it;
            }
        }
    }
    return files;
}
