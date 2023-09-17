#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <cstring>
using std::cout;
using std::endl;
using std::getline;
using std::ifstream;
using std::ofstream;
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

int search_string(const string file_path, const string str)
{
    string line;
    int counter;
    vector<string> file_data;
    ifstream search_file(file_path);

    if (!search_file.is_open())
        return -1; // file not opened

    while (getline(search_file, line))
        file_data.push_back(line);

    for (counter = 0; counter < file_data.size(); counter++)
    {
        if (strstr(file_data[counter].c_str(), str.c_str()))
            return 1; // string found
    }
    return 0; // sting not found
}

void search_string_from_files(const string directory_path, const string str)
{
    vector<string> extensions;
    vector<string> file_paths;
    ofstream note_file("note_file.txt");

    extensions = read_config("extensions.txt");     // read data from config and push that data into array of string
    traverse_directory(directory_path, file_paths); // traverse all the directory and get paths of regular files

    for (path file_path : file_paths)
    { // compare file extensions with our extension.txt config            // search string into file
        if (compare_extensions(file_path.filename().string(), extensions) && search_string(file_path.string(), str))
            note_file << file_path.filename().string() << endl; // note the names of file in which the string is found
    }
}

int main(int argc, char *argv[])
{
    search_string_from_files(argv[1], argv[2]);
    // C:\\Users\\"Vaibhav Gaikwad"\\Desktop\\Traverse_File_System\\test
    return 0;
}
