#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
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
    ifstream extension_file("extensions.txt");

    while (getline(extension_file, line))
        extensions.push_back(line);

    extension_file.close();
}

int count_files_by_extension(path file_path)
{
    static int count_files;
    static bool flag = true;
    static vector<string> extensions;

    if (flag)
    {
        flag = false;
        read_config(extensions);
    }

    try
    {
        for (auto file_name : directory_iterator(file_path))
        {
            if (is_regular_file(file_name.path()))
            {
                size_t dot_pos;
                string file_extensions;

                dot_pos = file_name.path().filename().string().find_last_of('.');

                if (dot_pos != string::npos)
                {
                    file_extensions = file_name.path().filename().string().substr(dot_pos);

                    for (int counter = 0; counter < extensions.size(); counter++)
                    {
                        if (!file_extensions.compare(extensions[counter]))
                            count_files++;
                    }
                }
            }

            if (is_directory(file_name.path()))
                count_files_by_extension(file_name.path());
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << endl;
    }

    return count_files;
}

int main(int argc, char *argv[])
{
    cout << count_files_by_extension(argv[1]) << endl;

    return 0;
}
