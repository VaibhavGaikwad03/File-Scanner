#include <iostream>
#include "File_Scanner.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <directory_path> <string_to_search>" << std::endl;
        exit(EXIT_FAILURE);
    }

    FileScanner scan;

    scan.scan_file(argv[1], argv[2]);

    std::cout << "\r\033[K";
    std::cout << "Done! Please check \'output.txt\'\n";

    return 0;
}