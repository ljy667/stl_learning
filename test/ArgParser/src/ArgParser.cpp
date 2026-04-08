#include "ArgParser.h"
#include <cstdlib>

//根据参数 适配  结构体 ，两者构成哈希映射
//不同情况选择赋值给结构体Arg合适的  value
//哈希可根据key 直接绑定value

namespace 
{
    std::string longNameTokey (const std::string &key){
        // -- output -> output
        return key.substr(2);  //获取从索引2开始的子字符串，即去掉前面的 "--"
    }   
} 
namespace cc
{
    void ArgParser::add_argument(const std::string &short_name ,
                                 const std::string &long_name ,
                                 const std::string &help , bool required){
        const std::string key = longNameTokey(long_name);
        //c++11 初始化列表（需要对Arg类定义构造函数）
        // 需要根据这个长参数名（key）匹配上对应的Arg类
        this->args_[key] = Arg{
                short_name,
                long_name,
                help,
                required,
                };

                //基于上述临时对象构建{} 直接构建的方式，可减少赋值开销？
                /* 
                 Arg arg; // 创建对象
                // 逐个赋值，顺序随意，可读性拉满
                arg.short_name = short_name;
                arg.long_name = long_name;
                arg.value = "";
                arg.help = help;
                arg.required = required;
                arg.is_flag = false;

                args_[key] = arg; // 存入map */
    }

    void ArgParser::add_flag(const std::string & short_name , 
                             const std::string & long_name,
                             const std::string &help){
        const std::string key = longNameTokey(long_name);
        Arg arg{short_name, long_name, help, false};
        arg.is_flag = true;
        this->args_[key] = std::move(arg);
    }


    void ArgParser::add_positional(const std::string& name,
                                   const std::string& help,
                                    bool required)
                                    {
                                        //处理pos_args
                                        this->pos_args.emplace_back(PosArg{name,help,required});
                                    }

    void ArgParser::print_help(std::string_view program_name){
        std::cout << program_name <<'\n' ;
    }
    
    //char *代表字符串  ，argv后＋[],即为 字符串的数组
    void ArgParser::parse(int argc , char *argv[])
    {
        // step1：就是把 native const char* 字符串数组 -> std::sting 
        // usage:argv[0] 是现在运行这段程序的程序名称，因此解析参数从argv[1]开始
        
         //收集位置参数
        std::vector<std::string_view> pos_arg_view;

        //遍历所有输入的字符串
        for (int i = 1 ; i < argc ; i++ )
        {
            std::string_view token{argv[i]};
            if(token == "--help" || token == "-h")
            {
                print_help(argv[0]);
                std::exit (0) ;
            }
            
            //长参数键值对解析
            // --key = value

            if(token.size() > 2 && token[0] == '-' && token[1] == '-'){
                auto equal_pos = token.find('=');
                if (equal_pos != std::string::npos)
                {
                    std::string_view key_view= token.substr(2,equal_pos-2);
                    std::string key {key_view};
                    auto it = this->args_.find(key);  //查看是否在键值对中
                    if  (it == args_.end()){
                        throw std::invalid_argument("unknown argment: --" + key);
                    }
                    it->second.value = token.substr(equal_pos+1);
                    it->second.is_set = true;  //成功解析，则设置is_set为true
                    continue;
                }
                else{
                    std::string_view key_view = token.substr(2);
                    std::string key{key_view};
                    auto it = this->args_.find(key);
                    if(it == args_.end()){
                        throw std::invalid_argument("unknown argment: --" + key);
                    }
                    if(it->second.is_flag)
                    {
                        it->second.is_set = true;
                    }else if(i+1 < argc){
                        it ->second.value = argv[++i]; 
                        //argv = ["prog", "--output", "out.txt"]  这里对应了这种输入类型
                        it->second.is_set = true;
                    }else{
                        throw std::invalid_argument("Missing value:" + std::string{token});
                    }
                    continue;
                }
            }
            if(token.size() >= 2 && token[0] == '-' && token[1] != '-'){
                std::string short_key = std::string {token};
                Arg* arg = FindLongbyShort(short_key);
                if(arg == nullptr){
                    throw std::invalid_argument("Unknown argument: " + short_key);
                }
                if(arg -> is_flag)
                {
                    arg-> is_set = true ;
                }
                else if (i+1 <argc)
                {
                    arg->value =argv[++i];
                    arg->is_set= true;
                }else {
                    throw std::invalid_argument("Missing value:" + short_key);
                }
                continue;
            }
            
            //token为已有的对象，效果与push_back一样
            //没有进入上述逻辑的，剩余的都是位置参数
            pos_arg_view.emplace_back(token); 
        }

        //
        for(size_t i =0 ; i < pos_args.size(); i++)
            {   
                if(pos_arg_view.size() > pos_args.size())
                {
                    throw std::invalid_argument("too many positional arguments");
                }
                if(i<pos_arg_view.size()){
                    //配置pos_values
                    pos_values[pos_args[i].name] = pos_arg_view[i];
                }else if(pos_args[i].required){
                    throw std::invalid_argument("Required Position args:<" + pos_args[i].name + ">");
                }
            } 
        // 校验长短参
        for(auto &[key,arg] : args_)
            {
                if(arg.required && !arg.is_set)
                {
                    throw std::invalid_argument("Required args:" + key);
                }
            }
    }
} // namespace cc






