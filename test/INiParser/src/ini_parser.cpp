#include "ini_parser.hpp"
#include "string_splits.hpp"
#include "trim.hpp"


/* 标准的ini文件
[Database]          ; 👈 这一行的 Database 就是【节名】
ip = 127.0.0.1
port = 3306
user = root

[User]              ; 👈 这一行的 User 就是【节名】
name = admin
password = 123456
 */

static cxx_utils::string::TrimPolicy Both ;

//处理每一行
bool IniParser::consume_line(const std::string_view& line,  std::string& current_section){
    
    //优先处理解节
    if(line.front() == '['){
        auto end_pos = line.find(']');
        if(end_pos == std::string::npos) 
            return false; //处理失败
        
        //auto section_name = line.substr(1,size(line)-2);
        auto section_name = line.substr(1,end_pos-1);
        section_name = trim(section_name ,Both);
        m_data.emplace(current_section,ini_section_t{});
        return true;
    }
    
    //处理节中的每一行=>处理键值对
    auto splited = cxx_utils::string::splits_fix(line , '=');
    if(splited.size() < 2)
        return false;
    std::string key{cxx_utils::string::trim(splited[0], Both)};
        if(key.empty()){
            return false;
        }
    std::string value{cxx_utils::string::trim(splited[1], Both)};
    m_data[current_section][key] = value;
    
}

//解析操作
bool IniParser::parse(const std::string_view view)
{
    using namespace cxx_utils::string;
    
    //clear();
    
    m_data.emplace("" , ini_section_t{}); //给最顶上的或者式空的节基于默认的键值对

    //分割换行符 
    auto lines = cxx_utils::string::splits_fix(view, '\n');
    std::string current_sections = "";

    //迭代每一行，处理每一行解析得到的结论
    //并且每一行需要处理前后空格
    for(const auto& line : lines)
    {
        auto trimmed = cxx_utils::string::trim(line, Both);
        if(trimmed.empty()){
            continue;
        }
        //处理每一行,确保每一行处理成功
        bool is_ok = consume_line(trimmed , current_sections);
        if(!is_ok)
        {
            return false;
        }
    }
    return true;
}


//既然需要转换，则直接传入std::string
std::optional<std::string> IniParser::get(const std::string& section , const std::string& key)
{
    //寻找第一层迭代器
    const auto ini_section_it = m_data.find(section);
    
    if(ini_section_it == m_data.end())
        {   
            //没找到
            return std::nullopt;
        }

    const auto value_it = ini_section_it -> second.find(key);
    if(value_it == ini_section_it ->second.end())
    {
        return std::nullopt;
    }
    return value_it -> second ;

}