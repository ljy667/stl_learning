#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <iterator>
#include <map>
//获取索引
//某些结构里索引没有意义
/* int main()
{
    std::vector<std::string> vec ;
    vec.rbegin();
    for (auto iter  =  vec.begin() ; iter != vec.end() ; iter++)
    {
        std::cout << * iter << "\n" ;   
    }
} */

//每个容器都必须提供线性遍历其数据结构的能力
 
void test_stream()
{
    std::ifstream in ("STL_LEARNING/src/data.in");
    std::istream_iterator<int> begin(in);
    std::istream_iterator<int> end;
    auto it = begin;
    //创建的文件流迭代器默认绑定到流的末尾后的一位，即默认为结束迭代器
    std::cout << *it++ <<"\n" ;

    begin++;
    while (begin != end)
    {
        std::cout << *begin++ << "\n" ;
    }
}

//检查映射表中是否有某元素的方法
void check_map ()
{
    std::map<std::string , double> m;
    std::pair <std::string, double> p1 {"bat" ,14.75};
    m.insert(p1) ;
    m.insert({"dat", 10.157});
    m.emplace("cat" ,10.157);
    
    auto iter = m.find("cat");
    if(iter != m.end())
    {
        std::cout << "value:" << iter->first <<"," <<iter->second<<"is there\n";
      }
    else{
        //没找到
    }
}

int main() 
{
    check_map() ;

}