#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <string.h>
#include "File_Scanner.h"
#include "Thread_Pool.h"

bool FileScanner::search(const std::string &file_path, const std::string &str)
{
    std::string line;
    std::string file_data;
    std::ifstream file(file_path);

    if (!file.is_open())
    {
        std::cerr << "Unable to open: "
                  << "\'" << file_path << "\'\n";
        file.close();
        return false; // Unable to open file
    }

    while (getline(file, line))
        file_data.append(line);
    file.close();

    if (strstr(file_data.c_str(), str.c_str()))
        return true; // String found
    return false;    // String not found
}

bool FileScanner::compare_extensions(const std::string &file_name)
{
    size_t dot_position;
    std::string file_extension;

    dot_position = file_name.find_last_of('.');

    if (dot_position != std::string::npos)
    {
        file_extension = file_name.substr(dot_position);

        for (const std::string &extension : extensions)
        {
            if (!file_extension.compare(extension))
                return true;
        }
    }
    return false;
}

std::vector<std::string> &FileScanner::traverse_directory(const fs::path &directory_path)
{
    for (const fs::directory_entry &entry : fs::recursive_directory_iterator(directory_path))
    {
        if (fs::is_regular_file(entry.path()))
            file_paths.push_back(entry.path().string());
    }
    return file_paths;
}

std::vector<std::string> &FileScanner::read_config(const std::string &config_path)
{
    std::string line;
    std::ifstream config_file(config_path);

    if (!config_file.is_open())
    {
        std::cerr << "Unable to open: "
                  << "\'" << config_path << "\'\n";
        exit(1);
    }

    while (getline(config_file, line))
        extensions.push_back(line);
    return extensions;
}

void FileScanner::scan_file(const std::string &directory_path, const std::string &str)
{
    ThreadPool pool(std::thread::hardware_concurrency());
    read_config("./config.txt");
    std::ofstream output_file("output.txt");

    std::cout << "Traversing through the file system...";
    std::cout.flush();
    traverse_directory(directory_path);
    std::cout << "\r\033[K";

    std::cout << "Searching for the data...";
    std::cout.flush();
    for (const std::string &file_path : file_paths)
    {
        pool.execute_task([&]
                          {
            if (compare_extensions(file_path))
            {
                if (search(file_path, str))
                {
                    std::unique_lock<std::mutex> lock(file_scan_mtx);
                    output_file << file_path << std::endl;
                }
            } });
    }
    pool.wait(); // Waiting for completion of the tasks
}