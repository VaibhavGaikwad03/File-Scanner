#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <cstring>
#include <string>
using std::cout;
using std::endl;
using std::getline;
using std::ifstream;
using std::string;
using std::vector;
namespace file_sys = std::filesystem;
using namespace file_sys;

vector<string> read_config(const string config_path)
{
    string line;
    static vector<string> extensions;
    ifstream extension_file(config_path); // opens file

    while (getline(extension_file, line)) // reading data line by line
        extensions.push_back(line);       // pushing readed data into array of string

    extension_file.close(); // close file

    return extensions;
}

void traverse_directory(const path directory_path, vector<string> &file_path)
{
    for (auto file_name : directory_iterator(directory_path))
    {
        if (is_regular_file(file_name.path()))              // if regular file
            file_path.push_back(file_name.path().string()); // then push file path into array of string

        if (is_directory(file_name.path()))                  // if directory file
            traverse_directory(file_name.path(), file_path); // call traverse_directory() recursively
    }
}

int compare_extensions(const string file_name, const vector<string> &extensions)
{
    size_t dot_pos;
    string file_extension;

    dot_pos = file_name.find_last_of('.'); // getting position of '.'

    if (dot_pos != string::npos) // if '.' found
    {
        file_extension = file_name.substr(dot_pos); // get the substring from the '.' eg. '.c', '.cpp', '.txt'

        for (string extension : extensions)
        {
            if (!file_extension.compare(extension)) // compare the extension
                return 1;
        }
    }
    return 0;
}

void list_files_by_extensions(const string directory_path)
{
    vector<string> extensions;
    vector<string> file_paths;

    extensions = read_config("extensions.txt");     // read data from config and push that data into array of string
    traverse_directory(directory_path, file_paths); // traverse all the directory and get paths of regular files

    for (path file_path : file_paths)
    { // compare file extensions with our extension.txt config
        if (compare_extensions(file_path.filename().string(), extensions))
            cout << file_path.filename().string() << endl;
    }
}

int main(int argc, char *argv[])
{
    list_files_by_extensions(argv[1]);
    // C:\\Users\\"Vaibhav Gaikwad"\\Desktop\\Traverse_File_System\\test
    
    return 0;
}
