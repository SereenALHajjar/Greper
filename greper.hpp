
#include <string>

namespace greper
{
    struct Config
    {
        bool parallel;
        bool printline;
        std::string dir;
        std::string keyword;
    };

    class Greper
    {

    public:
        Greper(Config);
        void searchInFile(const std::string &, const std::string &);
        bool checkDir(const std::string &);
        void printFoundMsg(const std::string &, int, const std::string &);
        void printErrorMsg(const std::string &);
        void searchInDirectorParallel(const std::string &, const std::string &);
        void searchInDirector(const std::string &, const std::string &);
        void run();
    private:
        Config conf_;
    };

}