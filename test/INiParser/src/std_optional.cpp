/* 
程序不能 稍微传个不规范的值，程序就整个崩溃

为什么需要 std :: optional
●许多函数的结果可能“有值”或“无值”
(解析失败、查找不到、空结果)。

·选择:返回哨兵值(0/-1/空字符串/指针)、
抛异常、或显式类型表示“可选”。

·std :: optional<T>能以类型系统表达
“要么有T,要么没有值”。
*/


/* Optional的用法
std :: optional<T> opt; // empty
. std :: optional<T> opt = std :: nullopt;
. std :: optional<T> opt = value; //有值
· opt.has_value()或 if(opt)检查
· *opt 或 opt.value()访问
获取值或默认
. opt.value_or(default)
. opt.reset()清空 */

#include <optional>
#include <string_view>
#include <charconv>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <iostream>

std::unordered_map<std::string, std::string>  map  ;
//解析一个整数
int parse_int_old (std::string_view int_str)
{
    int value  = 0 ;
    auto [ptr,ec]= 
        std::from_chars(int_str.data() , int_str.data()+ int_str.size(), value );
    if(ec == std::errc()){
        return value;
    }
    else{
        return -1;  //关键缺陷：无法区分输入本身是 - 1和转换失败两种场景
    }
}

std::string get_env(std::string_view sv)
{   //底层const, 迭代器指向的对象不可修改
    map.insert({
    {"key1", "val1"},
    {"key2", "val2"},
    {"key3", "val3"} });
    const auto it = map.find (std::string{sv});
    if(it != map.end() ){
        return it->second;
    }
    else{
        throw std::invalid_argument("No Target" + std::string{sv} + "value Found");
    }
}

//因返回的对象可能有问题，可能没问题

//std::optional提供了更强的语义接口
//有效就把值拿走，无效就进行语义异常处理

std::optional<int> parse_int(std::string_view sv)
{
    int value = 0;
    auto [ptr ,ec] = std::from_chars (sv.data(), sv.data()+sv.size() ,value );
    if(ec == std::errc())
    {   
        //如果成功，int 可隐式转换为std::optional<int>
        return value;
    }
    return std::nullopt;
}

int main()
{
    auto value = parse_int("1234");
    std::cout << *value << std::endl;

    auto value1 = parse_int("fcaaavsva");
    if(value1)
    {
    std::cout << *value1 << std::endl;
    }
    else{
        std::cerr << "Error providing the string"<<std::endl;
    }

    value = parse_int("asdasdzxc").value_or(0);
    std::cout <<*value ;
 }

/*  注意一些事情:

· *opt 与 opt.value():
· *opt 在未定义行为下不检查。
· opt.value()在没有值时会抛出std :: bad_optional_access
(谨慎使用)。

· 更安全的访问:先检查 if(opt)或 opt.has_value()。
. value_or(default)非常便利,用于提供安全的后备值。 */

