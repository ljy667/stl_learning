#include <string>
#include <unordered_map>

#pragma onece

namespace cc {
//argc: 参数数量 argv：参数数组本体
//短参： -o
//长参： --output
//值：跟在后面的
//

// required
// set？  （解析时是否构成键值对）
// 是不是flag？

    struct Arg
    {
        std::string short_name;
        std::string long_name;
        std::string value;
        
        std::string help;

        bool required{false};
        bool is_flag{false};
        
        private:
        bool is_set{false};
    };
    //聚合？

    //若不仅需要存储解析的东西，并且需要承担一些方法，用class

    class ArgParser {
        public:
        ArgParser() = default;  //->parse

        void parse(int argc, char *argv[]);
        
        void add_argument(const std::string &short_name , const std::string &long_time,
                            const std::string &help , bool required = false);

        //interfaces
        
        private:
        std::unordered_map<std::string , Arg>  
        
        

    };
};