#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <argparse/argparse.hpp>
#include <thread>
#include <mutex>
using namespace std;
using namespace filesystem;

#define RED "\033[31m"
#define BLUE "\033[34m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define RESET "\x1b[0m"

struct Config {
 bool parallel ;
 bool printline ;
 string dir ;
 string keyword ;
}


void searchInFile(const string &, const string &);
bool checkDir(const string &);
void printFoundMsg(const string &, int, const string &);
void printErrorMsg(const string &);
void searchInDirectorParallel(const string &, const string &);
void searchInDirector(const string &, const string &);

int main(int argc, char *argv[])
{

    argparse::ArgumentParser program("Greper");
    program.add_argument("-d", "--dir").required();
    program.add_argument("-p", "--parallel").default_value(false).implicit_value(true);

    program.add_argument("keyword");
    try
    {
        program.parse_args(argc, argv);
    }
    catch (const exception &err)
    {
        cerr << err.what() << endl;
        cerr << program;
        return 1;
    }
    string dir_path = program.get<string>("-d");
    string keyword = program.get<string>("keyword");
    if (checkDir(dir_path))
    {
        if (program["--parallel"] == true)
            searchInDirectorParallel(dir_path, keyword);
        else
            searchInDirector(dir_path, keyword);
    }
    else
        printErrorMsg("Direction Doesn't Exist or It Isn't a Directory");
}

void searchInFile(const string &file_path, const string &keyword)
{
    ifstream file(file_path);
    if (!file.is_open())
    {
        printErrorMsg("Unable to open file: " + file_path + " for reading.");
        return;
    }
    string line;
    int line_num = 1;
    while (getline(file, line))
    {
        if (line.find(keyword) != string::npos)
        {
            printFoundMsg(file_path, line_num, line);
        }
        line_num++;
    }
}
bool checkDir(const string &dir_path)
{
    return exists(dir_path) && is_directory(dir_path);
}
void printFoundMsg(const string &file_path, int line_num, const string &line)
{
    printf("%s%s:%s%d%s%s\n", GREEN, file_path.c_str(), BLUE, line_num, RESET, line.c_str());
}
void printErrorMsg(const string &msg)
{
    printf("%s%s%s\n", RED, msg.c_str(), RESET);
}
void searchInDirectorParallel(const string &dir_path, const string &keyword)
{
    vector<thread> threads;
    for (const auto &entry : recursive_directory_iterator(dir_path))
    {
        if (entry.is_regular_file())
        {
            string entry_path = entry.path();
            threads.push_back(thread(
                [keyword, entry_path]()
                {
                    searchInFile(entry_path, keyword);
                }));
        }
    }
    for (auto &thr : threads)
    {
        thr.join();
    }
}
void searchInDirector(const string &dir_path, const string &keyword)
{
    for (const auto &entry : recursive_directory_iterator(dir_path))
    {
        if (entry.is_regular_file())
        {
            searchInFile(entry.path(), keyword);
        }
    }
}