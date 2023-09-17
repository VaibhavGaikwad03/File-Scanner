#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <cstring>
using std::cout;
using std::endl;
using std::getline;
using std::ifstream;
using std::string;
using std::vector;
namespace file_sys = std::filesystem;
using namespace file_sys;

void read_config(vector<string> &extensions)
{
    string line;
    ifstream extension_file("extensions.txt"); // opens file
    if (!extension_file.is_open())
        return;

    while (getline(extension_file, line)) // reading data line by line
        extensions.push_back(line);       // pushing readed data into array of string

    extension_file.close(); // close file
}

int search_string(string file_path, string str)
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

void search_string_from_files(path absolute_path, string str)
{
    static bool flag = true;
    static vector<string> extensions;

    if (flag) // for execute the code only once
    {
        flag = false;
        read_config(extensions); // read config file data into 'extensions' array
    }

    try
    {
        for (auto file_path : directory_iterator(absolute_path)) // directory iterator iterates through the directory and give file path in 'file_path'
        {
            if (is_regular_file(file_path.path())) // checks that file is regular file or not
            {
                size_t dot_pos;
                string file_extensions;

                dot_pos = file_path.path().filename().string().find_last_of('.'); // searching for '.' in file names

                if (dot_pos != string::npos)
                {
                    file_extensions = file_path.path().filename().string().substr(dot_pos); // extracting substring from file name i.e. file extension

                    for (int counter = 0; counter < extensions.size(); counter++)
                    {
                        if (!file_extensions.compare(extensions[counter])) // checking if file extension of file and extensions from config file are matching or not
                        {
                            if (search_string(file_path.path().string(), str))        // if string found in file
                                cout << file_path.path().filename().string() << endl; // display that file name
                        }
                    }
                }
            }

            if (is_directory(file_path.path()))                 // checking if the file if directory file or not
                search_string_from_files(file_path.path(), str); // if directory file then call list_file_by_extension recursively
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << endl;
    }
}

int main(int argc, char *argv[])
{
    search_string_from_files(argv[1], argv[2]);
    // C:\\Users\\"Vaibhav Gaikwad"\\Desktop\\Traverse_File_System\\test
    return 0;
}
