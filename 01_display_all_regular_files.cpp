#include <iostream>
#include <filesystem>
using std::cout;
using std::endl;
namespace file_sys = std::filesystem;
using namespace file_sys;

void traverse_directory(path file_path)
{
    for (auto file_name : directory_iterator(file_path))
    {
        if (is_regular_file(file_name.path()))
            cout << file_name.path().filename().string() << endl;

        if (is_directory(file_name.path()))
            traverse_directory(file_name.path());
    }
}

int main(void)
{
    path file_path = "C:\\Users\\Vaibhav Gaikwad\\Desktop\\Traverse_File_System\\test";

    traverse_directory(file_path);

    return 0;
}
