#include <bits/stdc++.h>
#include <filesystem>
#include <fstream>

using namespace std;
using namespace filesystem;

#define RED "\033[31m"
#define BLUE "\033[34m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"

string dirPath, keyword;
bool found;
void SearchInFile(string filepath)
{
    ifstream file(filepath);
    if (!file.is_open())
    {
        cout << RED << "Unable to open file: " << filepath << " for reading." << endl;
        return;
    }
    string line;
    int lineNum = 1;
    while (getline(file, line))
    {
        if (line.find(keyword) != string::npos)
        {
            found = 1;
            cout << GREEN << "Found in file: " << BLUE << filepath << GREEN << " at line " << YELLOW << lineNum << endl;
        }
        lineNum++;
    }
}
bool checkDir()
{
    return exists(dirPath) && is_directory(dirPath);
}
void SearchInDirector()
{
    if (!checkDir())
    {
        cout << RED << "Direction Doesn't Exist or It Isn't a Directory" << endl;
        return;
    }
    for (const auto &entry : recursive_directory_iterator(dirPath))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".txt")
        {
            SearchInFile(entry.path());
        }
    }
}
int main(int argc, char *argv[])
{
    if (argc == 4)
    {
        if (string(argv[1]) == "--dir" || string(argv[1]) == "-d")
        {
            dirPath = argv[2];
            keyword = argv[3];
            SearchInDirector();
            if (!found)
                cout << YELLOW << "Word Doesn't Exist" << endl;
        }
        else
            cout << RED << "syntax error" << endl;
    }
    else
        cout << RED << "syntax error" << endl;
}