#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <utility>
#include <string_view>
#pragma once



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
        friend class ArgParser;
        public:
        std::string short_name;
        std::string long_name;
        std::string value;
        
        std::string help;

        bool required{false};
        bool is_flag{false};
        
         // ✅ 核心修复：显式声明【无参默认构造】
        Arg() = default;

        // 兼容c++11 {} 的临时变量构建对象
        Arg(std::string s_name , std::string l_name, std::string h , bool req)
         :short_name(std::move(s_name)),
         long_name(std::move(l_name)),
         value(""),
         help(std::move(h)),
         required(req),
         is_flag(false){}


        private:
        //标记位，标记用户是否真正传入某个参数
        bool is_set{false};
    };

    //位置参数
    struct PosArg{
        std::string name;
        std::string help;
        bool required;
    };

    //聚合？

    //若不仅需要存储解析的东西，并且需要承担一些方法，用class

    class ArgParser {
        public:
        ArgParser() = default;  //->parse

        void parse(int argc, char *argv[]);

        //interfaces
        
        // "-o" , "--out"
        void add_argument(const std::string &short_name , const std::string &long_time,
                            const std::string &help , bool required = false);
        
        void add_flag(const std::string &short_name,
                    const std::string &long_name ,const std::string &help);
        
        template<typename T>
        T  get(const std::string & key) const{
            auto it = args_.find(key);
            //如果找到这个key,并且这个key对应的value不是空的
            if(it != args_.end() && !it->second.value.empty()){
                T result;
                std::istringstream(it->second.value) >> result;
                return result;
            }

            auto pit = pos_values.find(key);
            if(pit != pos_values.end() )
            {
                T result;
                std::istringstream(pit->second)  >>result;
            }
            throw std::runtime_error("Argument not found");  
            /* 拿到一个字符串（比如 "14.75"）
            把它包装成输入流
            用 >> 把它读进 T 类型变量（T 可以是 int、double、float…） */
        }

        //pos_args, 设置pos_args容器的各个PosArg
        void add_positional(const std::string& name , const std::string& help,
                            bool required);


        private:
        std::unordered_map<std::string , Arg> args_ ;  
        std::vector<PosArg> pos_args;
        std::unordered_map<std::string, std::string> pos_values;     
        //打印输出帮助
        //void print_help(const char* program_name) ;
        void print_help(std::string_view program_name);

        Arg* FindLongbyShort(const std::string& name){
            for(auto& [k,v] :args_){
                if(v.short_name == name)
                {
                    return &v;
                }
            }
            return nullptr;
        }
    };
};

