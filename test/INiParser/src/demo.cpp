
#include "string_splits.hpp"

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

}