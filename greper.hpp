
#include <string>
#include<vector>
#include<mutex>
namespace greper
{
    struct Config
    {
        bool parallel;
        bool printline;
        bool count ; 
        std::string dir;
        std::string keyword;
    };

    class Greper
    {

    public:
        Greper(Config);
        void run();

    private:
        void searchInFile(const std::string &);
        bool checkDir();
        void printFoundMsgWithLine(const std::string &, int, const std::string &);
        void printFoundMsg(const std::string &, int);
        void printErrorMsg(const std::string &);
        void printCounts() ;
        void searchInDirectorParallel();
        void searchInDirectory();
        void search(const std::string &);
        
    private:
        Config conf_;
        std::vector<std::pair<std::string , uint64_t>>counts ; 
        std::mutex mtx ;
    };

}