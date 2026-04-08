//怎样可以不暴露实现，但是可告诉别人接口，什么功能可以
//模板

//split 模块， 应当接收字符串，返回分割结果

#pragma once    
#include <vector>
#include <string>
#include <iostream>
#include <string_view>

namespace cxx_utils{
    namespace string{
    //interface of string splits
    std::vector<std::string_view> splits(const std::string& src , const char ch);
    std::vector<std::string_view> splits_fix(const std::string_view src , const char ch);

    }
}

