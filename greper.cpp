
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
    uint64_t countSubstringOccurrences(const std::string &str, const std::string &substr)
    {
        uint64_t count = 0;
        size_t pos = 0;
        while ((pos = str.find(substr, pos)) != std::string::npos)
        {
            ++count;
            pos += substr.length();
        }

        return count;
    }

    void Greper::search(const std::string& file_path)
    {
        std::ifstream file(file_path);
        if (!file.is_open())
        {
            printErrorMsg("Unable to open file: " + file_path + " for reading.");
            return;
        }
        string line;
        uint64_t total_count = 0;
        for (uint64_t line_num = 1;getline(file, line); line_num++)
        {
            uint64_t cur_count = countSubstringOccurrences(line, conf_.keyword);
            if (cur_count == 0) continue;

            if (conf_.printline)
                printFoundMsgWithLine(file_path, line_num, line);
            else
                printFoundMsg(file_path, line_num);
            total_count += cur_count;
        }
        if (conf_.count)
        {
            std::lock_guard<std::mutex> lock(mtx);
            this->counts.push_back({file_path, total_count});
        }

    }
    void Greper::searchInFile(const string &file_path)
    {
        std::ifstream file(file_path);
        if (!file.is_open())
        {
            printErrorMsg("Unable to open file: " + file_path + " for reading.");
            return;
        }
        string line;
        uint64_t line_num = 1;
        while (getline(file, line))
        {
            if (line.find(this->conf_.keyword) != string::npos)
            {
                if (conf_.printline)
                    printFoundMsgWithLine(file_path, line_num, line);
                else
                    printFoundMsg(file_path, line_num);
                //count += countSubstringOccurrences(line, conf_.keyword);

            }
            line_num++;
        }
    }
    bool Greper::checkDir()
    {
        return exists(conf_.dir) && is_directory(conf_.dir);
    }
    void Greper::printFoundMsgWithLine(const string &file_path, int line_num, const string &line)
    {
        printf("%s%s:%s%d%s%s\n", GREEN, file_path.c_str(), BLUE, line_num, RESET, line.c_str());
    }
    void Greper::printFoundMsg(const string &file_path, int line_num)
    {
        printf("%s%s:%s%d%s\n", GREEN, file_path.c_str(), BLUE, line_num, RESET);
    }
    void Greper::printErrorMsg(const string &msg)
    {
        printf("%s%s%s\n", RED, msg.c_str(), RESET);
    }
    void Greper::searchInDirectorParallel()
    {
        std::vector<std::thread> threads;
        for (const auto &entry : recursive_directory_iterator(conf_.dir))
        {
            if (entry.is_regular_file())
            {
                string entry_path = entry.path();
                threads.push_back(std::thread(
                    [this, entry_path]()
                    {
                        this->search(entry_path);
                    }));
            }
        }
        for (auto &thr : threads)
        {
            thr.join();
        }
    }
    void Greper::searchInDirectory()
    {
        for (const auto &entry : recursive_directory_iterator(conf_.dir))
        {
            if (entry.is_regular_file())
            {
                search(entry.path());
            }
        }
    }
    void Greper::printCounts()
    {
        uint64_t tot = 0;
        for (auto [filename, cnt] : counts)
        {
            printf("%s%s count = %s%lu%s\n", GREEN, path(filename).filename().c_str(), YELLOW, cnt, RESET);
            tot += cnt;
        }
        printf("total = %lu\n", tot);
    }
    void Greper::run()
    {
        if (checkDir())
        {
            if (conf_.parallel)
                searchInDirectorParallel();
            else
                searchInDirectory();
            if (conf_.count)
                printCounts();
        }
        else
            printErrorMsg("Direction Doesn't Exist or It Isn't a Directory");
    }
}