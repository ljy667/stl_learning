
#include "string_splits.hpp"
#include "trim.hpp"


bool satisfiesConditions(std::vector<std::vector<int>>& grid) {
    //迭代矩阵每一个元素
    for(int i=0;i<grid.size();i++){
        for(int j=0;i<grid[0].size();j++)
        {
            if(i+1 <grid.size() && grid[i][j] != grid[i+1][j]){
                return  false;
            }
            if(j+1 < grid[0].size() && grid[i][j] == grid[i][j+1]){
                return false;
            }
        }
    }
    return true;
}


int main()
{
    using namespace cxx_utils::string;
    std::string kv {"key=value"};
    auto result = splits_fix(kv, '=');

    //遍历vector
    for(const auto& each_result : result)
    {
        std::cout << each_result << "\n" ;
    }
    auto trimpolicy=TrimPolicy::Left;
    std::string str_temp = "   saadsdad   ";
    const auto result_v1 = trim(str_temp,trimpolicy);

    std::cout << result_v1 << std::endl;
}

