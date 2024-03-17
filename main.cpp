#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <argparse/argparse.hpp>
#include <thread>
#include <mutex>
#include "greper.hpp"
using namespace std;
using namespace filesystem;

#define RED "\033[31m"
#define BLUE "\033[34m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define RESET "\x1b[0m"

int main(int argc, char *argv[])
{

    argparse::ArgumentParser program("Greper");
    program.add_argument("-d", "--dir").required();
    program.add_argument("-p", "--parallel").default_value(false).implicit_value(true);
    program.add_argument("-l", "--line").default_value(false).implicit_value(true);
    program.add_argument("-c", "--count").default_value(false).implicit_value(true);

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
    greper ::Config conf;
    conf.printline = program["-l"] == true;
    conf.parallel = program["-p"] == true;
    conf.count = program["-c"] == true ;
    conf.keyword = program.get<string>("keyword");
    conf.dir = program.get<string>("-d");
    greper ::Greper greper(conf);
    greper.run();
}