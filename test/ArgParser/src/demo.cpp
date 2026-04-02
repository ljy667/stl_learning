#include "ArgParser.h"


int main(int argc , char* argv[])
{
    std::string a = "-o";
    cc::ArgParser parser;
    parser.add_argument(a, "--output" , "Output Position",true);
    parser.add_argument("-c" , "--count" , "count of file" ,true);
    parser.add_positional ("input" , "help" , true);
    try
    {
        parser.parse(argc , argv);
        int a  = parser.get<int>("count");
            std::cout <<a<<'\n'  ;

        const std::string input = parser.get<std::string>("input");
        std::cout << input << '\n' ;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
   
  
}
