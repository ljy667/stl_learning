#include "trim.hpp"
#include <cctype>

namespace{
    inline bool is_space_char(const char ch){
        return std::isspace(ch) != 0;
    }
}

namespace cxx_utils::string
{
    std::string_view trim(const std::string_view sv,const TrimPolicy trim_policy){
    //找到第一个非空白字符index
    //从后往前找，第一个非空白字符的index
    //[f,e]
    
    //输入检查
    if(sv.empty()){return sv;}
    size_t end  =  sv.size();
    size_t start = 0;
    
    // "   ASDASDASZX  "
    
    while(start < end  && is_space_char(sv[start]))
    {
        start++;
    }
     
    while (end > start && is_space_char(sv[end-1]))
    {
        end -- ;
    }
    

    }
} // namespace cxx_utils::string
