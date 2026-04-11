
#include "string_splits.hpp"
#include "trim.hpp"

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

