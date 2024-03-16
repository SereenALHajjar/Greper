
#include <vector>
#include <argparse/argparse.hpp>



#include "greper.hpp"

using namespace std;




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

    greper::Config conf;
    conf.parallel = (program["--parallel"] == true);
    conf.dir = rogram.get<string>("-d");
    conf.keyword = program.get<string>("keyword");
    conf.printline = false; // TODO(Sereen): parse this from command line
    
    greper::Greper greper(conf);
    greper.run();


}
