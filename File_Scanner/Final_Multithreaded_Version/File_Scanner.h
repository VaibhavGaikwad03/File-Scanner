#ifndef FILE_SCANNER_H
#define FILE_SCANNER_H

#include <mutex>
#include <vector>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

class FileScanner
{
    std::mutex file_scan_mtx;
    std::vector<std::string> file_paths;
    std::vector<std::string> extensions;

    // private:
    bool search(const std::string &file_path, const std::string &str);
    bool compare_extensions(const std::string &file_path);
    std::vector<std::string> &traverse_directory(const fs::path &directory_path);
    std::vector<std::string> &read_config(const std::string &config_path);

public:
    void scan_file(const std::string &directory_path, const std::string &str);
};

#endif // FILE_SCANNER_H