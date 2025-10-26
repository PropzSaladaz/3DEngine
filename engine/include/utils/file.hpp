#pragma once

#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>

#define FILE_DOESNT_EXIST "FILE_DOESNT_EXIST"

namespace file {
    namespace fs = std::filesystem;

    inline fs::path exe_dir() {
        // Linux: resolve the running executable, then take its directory
        return fs::canonical("/proc/self/exe").parent_path();
    }

    // Builds "<exe_dir>/resources/<relativeFilePath>"
    inline fs::path resource_path(const std::string& relativeFilePath) {
        return exe_dir() / "resources" / fs::path(relativeFilePath);
    }

    // Reads a text file from resources/, returns contents or "FILE_DOESNT_EXIST"
    inline std::string readFile(const std::string& relativeFilePath) {
        const fs::path fullPath = resource_path(relativeFilePath);

        std::cout << "reading from file: " << fullPath.string() << std::endl;

        std::ifstream in(fullPath);
        if (!in.is_open()) return "FILE_DOESNT_EXIST";

        std::ostringstream buf;
        buf << in.rdbuf();
        return buf.str();
    }
} // namespace file