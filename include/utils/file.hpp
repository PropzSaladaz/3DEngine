#ifndef FILE_HPP
#define FILE_HPP

#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>

#define FILE_DOESNT_EXIST "FILE_DOESNT_EXIST"

namespace fs = std::filesystem;

namespace file {
    static inline std::string getFullPathFromRelative(const std::string& relativeFilePath) {
        fs::path sourcePath = fs::current_path().parent_path().parent_path().parent_path();
        std::string fullPath = (sourcePath / relativeFilePath).string();
        return fullPath;
    }

    static inline std::string readFile(const std::string& relativeFilePath) {
        std::string fullPath = getFullPathFromRelative(relativeFilePath);
        std::cout << "reading from file: " << fullPath << std::endl;
        std::ifstream file(fullPath);
        if (!file.is_open()) return FILE_DOESNT_EXIST;
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string fileContents = buffer.str();
        file.close();
        return fileContents;
    }
} // namespace file

#endif