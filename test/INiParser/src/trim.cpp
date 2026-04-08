#include "trim.hpp"
#include <cctype>

namespace{
    inline bool is_space_char(const char ch){
        // std::isspace is undefined for negative signed char values
        //return std::isspace(static_cast<unsigned char>(ch)) != 0;
        return std::isspace(ch) != 0;

    }
}

namespace cxx_utils::string
{
    std::string_view trim(const std::string_view sv , const TrimPolicy trim_policy){
    //找到第一个非空白字符index
    //从后往前找，第一个非空白字符的index
    //[f,e]
    
    //输入检查
        if(sv.empty()){return sv;}
        size_t end  =  sv.size();
        size_t start = 0;
        
        // "   ASDASDASZX  "
        if(trim_policy == TrimPolicy::Both || trim_policy == TrimPolicy::Left)
        {
            while(start < end  && is_space_char(sv[start]))
            {
                start++;
            }
        }
        
        //trim函数中 ， TrimPolicy
        if(trim_policy == TrimPolicy::Both || trim_policy == TrimPolicy::Right)
        {
            while (end > start && is_space_char(sv[end-1]))
            {
                end -- ;
            }
        }
        return sv.substr(start, end - start);
    }
} // namespace cxx_utils::string
