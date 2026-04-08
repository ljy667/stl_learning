#include "string_splits.hpp"
#include <algorithm>

namespace cxx_utils{
    namespace string{
    //interface of string splits
    std::vector<std::string_view> splits(const std::string& src , const char ch){
        static constexpr const int START_POS = 0 ;
        auto current_equal_pos = src.find(ch , START_POS);
        size_t last_index = START_POS;

        //std::vector <std::string> result ;
        std::vector <std::string_view> result ;
        //改进2  预留空间
        //delimiter 定界符
        const size_t delim_count = std::count (src.begin() ,src.end() ,ch);
        result.reserve( 2*delim_count + 1);
 
        last_index = START_POS;
        while (current_equal_pos != std::string::npos)
        {
            result.emplace_back(src.substr(last_index,current_equal_pos -last_index));
            //将分割符自身传进去
            //result.emplace_back(std::string(1,ch));  //将单个字符 ch 转换为长度为 1 的字符串，原地构造并添加到 result 容器末尾。
            result.emplace_back(src.substr(current_equal_pos,1));
            last_index = current_equal_pos + 1 ;

            //顺找下一个分割符
            current_equal_pos =  src.find (ch , last_index);
        }
        result.emplace_back(src.substr(last_index)); //一直到结尾的字符串
        return result;
    }


    //如果能知道在哪里分割， 如果目标不为存储分割后的子字符， 而是获取分割位置
    
    //上述split需要 构建较多std::string 对象

    //迭代 stirng_view

    std::vector<std::string_view> splits_fix(const std::string_view src , const char ch){
        static constexpr const int START_POS = 0 ;
        auto current_equal_pos = src.find(ch , START_POS);
        size_t last_index = START_POS;

        //std::vector <std::string> result ;
        std::vector <std::string_view> result ;
        //改进2  预留空间
        //delimiter 定界符
        const size_t delim_count = std::count (src.begin() ,src.end() ,ch);
        result.reserve( 2*delim_count + 1);
 
        last_index = START_POS;
        while (current_equal_pos != std::string::npos)
        {
            result.emplace_back(src.substr(last_index,current_equal_pos -last_index));
            //将分割符自身传进去
            //result.emplace_back(std::string(1,ch));  //将单个字符 ch 转换为长度为 1 的字符串，原地构造并添加到 result 容器末尾。
            result.emplace_back(src.substr(current_equal_pos,1));
            last_index = current_equal_pos + 1 ;

            //顺找下一个分割符
            current_equal_pos =  src.find (ch , last_index);
        }
        result.emplace_back(src.substr(last_index)); //一直到结尾的字符串
        return result;
    }

    }
}
