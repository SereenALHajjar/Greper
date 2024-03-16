
#include "greper.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <thread>
#include <mutex>
#include <vector>

#define RED "\033[31m"
#define BLUE "\033[34m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define RESET "\x1b[0m"

using std::string;
using namespace std::filesystem;

namespace greper
{

    Greper::Greper(Config config)
    {
        conf_ = config;
    }

    void Greper::searchInFile(const string &file_path, const string &keyword)
    {
        std::ifstream file(file_path);
        if (!file.is_open())
        {
            printErrorMsg("Unable to open file: " + file_path + " for reading.");
            return;
        }
        string line;
        size_t line_num = 1;
        while (getline(file, line))
        {
            if (line.find(keyword) != string::npos)
            {
                printFoundMsg(file_path, line_num, line);
            }
            line_num++;
        }
    }
    bool Greper::checkDir(const string &dir_path)
    {
        return exists(dir_path) && is_directory(dir_path);
    }
    void Greper::printFoundMsg(const string &file_path, int line_num, const string &line)
    {
        printf("%s%s:%s%d%s%s\n", GREEN, file_path.c_str(), BLUE, line_num, RESET, line.c_str());
    }
    void Greper::printErrorMsg(const string &msg)
    {
        printf("%s%s%s\n", RED, msg.c_str(), RESET);
    }
    void Greper::searchInDirectorParallel(const string &dir_path, const string &keyword)
    {
        std::vector<std::thread> threads;
        for (const auto &entry : recursive_directory_iterator(dir_path))
        {
            if (entry.is_regular_file())
            {
                string entry_path = entry.path();
                threads.push_back(std::thread(
                    [this, keyword, entry_path]()
                    {
                        this->searchInFile(entry_path, keyword);
                    }));
            }
        }
        for (auto &thr : threads)
        {
            thr.join();
        }
    }
    void Greper::searchInDirector(const string &dir_path, const string &keyword)
    {
        for (const auto &entry : recursive_directory_iterator(dir_path))
        {
            if (entry.is_regular_file())
            {
                searchInFile(entry.path(), keyword);
            }
        }
    }

    void Greper::run()
    {
        // if (checkDir(dir_path))
        // {
        //     if (program["--parallel"] == true)
        //         searchInDirectorParallel(dir_path, keyword);
        //     else
        //         searchInDirector(dir_path, keyword);
        // }
        // else
        //     printErrorMsg("Direction Doesn't Exist or It Isn't a Directory");
    }
}