/**
 * @file File_Scanner.cpp
 * @author Vaibhav Gaikwad (im.vaibhavtg07@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-04-11
 *
 * @copyright Copyright (c) 2024
 */
 
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>
#include <cstring>

namespace fs = std::filesystem;

/**
 * @brief Read extensions from a configuration file.
 *
 * This function reads extensions from the specified configuration file and appends them to the given vector.
 *
 * @param config_path The path to the configuration file.
 * @param extensions A vector to store the extensions read from the config file.
 * @return A vector of strings containing file extensions.
 *
 * @author Vaibhav Gaikwad
 */

std::vector<std::string> &read_config(const std::string &config_path, std::vector<std::string> &extensions)
{
    std::string line;
    std::ifstream config_file(config_path); // Opens config file

    if (!config_file.is_open()) // Error checking
    {
        std::cerr << "Unable to open a file: " << config_path << std::endl;
        return extensions;
    }

    while (getline(config_file, line)) // Reading data line by line
        extensions.push_back(line);
    config_file.close(); // Close opened file

    return extensions;
}

/**
 * @brief Traverse a directory recursively and collect file paths.
 *
 * This function traverses the specified directory recursively and collects paths of all regular files found within it and its subdirectories.
 *
 * @param directory_path The path to the directory to traverse.
 * @param file_path A vector to store the paths of the regular files found.
 *
 * @author Vaibhav Gaikwad
 */
std::vector<std::string> &traverse_directory(const fs::path &directory_path, std::vector<std::string> &file_path)
{
    for (fs::directory_entry file : fs::directory_iterator(directory_path))
    {
        if (fs::is_regular_file(file.path()))           // If regular file
            file_path.push_back(file.path().string());  // Push file path into vector of file paths
        if (fs::is_directory(file.path()))              // If directory file
            traverse_directory(file.path(), file_path); // Call traverse_directory() recursively
    }
    return file_path;
}

/**
 * @brief Compare a file's extension with a list of extensions.
 *
 * This function takes a file name and a list of extensions, and checks whether the file's extension matches any of the extensions in the list.
 *
 * @param file_name The name of the file whose extension is to be compared.
 * @param extensions A vector of strings containing the extensions to compare against.
 * @return Returns true if the file extension matches any of the provided extensions, otherwise returns false.
 */
bool compare_extensions(const std::string &file_name, const std::vector<std::string> &extensions)
{
    size_t dot_position;
    std::string file_extension;

    dot_position = file_name.find_last_of('.'); // Get position of '.'

    if (dot_position != std::string::npos) // '.' found
    {
        file_extension = file_name.substr(dot_position); // Get the extension of file e.g. .c, .cpp, .txt

        for (std::string extension : extensions)
        {
            if (!file_extension.compare(extension)) // Compare the extension
                return true;
        }
    }
    return false;
}

/**
 * @brief Search for a specific string in a file.
 * 
 * This function searches for a given string within the contents of a file specified by the file path. It returns whether the string is found in the file.
 * 
 * @param file_path The path of the file to be searched.
 * @param str The string to search for within the file.
 * @return Returns 1 if the string is found in the file, 0 if not found, and -1 if the file cannot be opened.
 * 
 * @author Vaibhav Gaikwad
 */
int search(const std::string &file_path, const std::string &str)
{
    std::string line;
    std::string file_data;
    std::ifstream file(file_path);

    if (!file.is_open())
    {
        std::cerr << "Unable to open a file: " << file_path << std::endl;
        return -1; // File not opened
    }

    while (getline(file, line))
        file_data.append(line);

    if (strstr(file_data.c_str(), str.c_str()))
        return 1; // String found
    return 0;     // String not found
}

/**
 * @brief Search for a specific string in files within a directory and its subdirectories.
 * 
 * This function searches for a given string within files located in a specified directory and its subdirectories. It uses a configuration file ("config.txt") to determine the file extensions to search in, traverses the directory recursively, and saves paths of file containing the string to an output file ("Output.txt").
 * 
 * @param directory_path The path to the directory to search.
 * @param str The string to search for within the files.
 * 
 * @author Vaibhav Gaikwad
 */
void search_string(const std::string &directory_path, const std::string &str)
{
    std::vector<std::string> extensions;
    std::vector<std::string> file_paths;
    std::ofstream output_file("output.txt");

    read_config("config.txt", extensions); // Read data from config and push that data into the vector of strings

    traverse_directory(directory_path, file_paths); // Traverse all the directories and get paths of regular files

    for (fs::path file_path : file_paths)
    {
        // Compares file extensions with our Config.txt config

        if (compare_extensions(file_path.filename().string(), extensions) && search(file_path.string(), str))
            output_file << file_path.string() << std::endl; // Note the paths of files in which the string is found
    }
}

int main(int argc, char *argv[])
{
    search_string(argv[1], argv[2]);

    return 0;
}
