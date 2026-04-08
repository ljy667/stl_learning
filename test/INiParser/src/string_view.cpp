/* 为什么需要 std :: string_view?

· std :: string 传参通常意味着一次拷贝或一次分配
· const std :: string&虽避免拷贝,但限制了调用方类型
·C风格字符串(const char*)不安全、长度未知
· 我们需要:统一接口+高性能+不拥有数据 */



/* 什么是std :: string_view

·本质:指向字符序列的一段“视图”
·内部只保存:指针+长度
·不拥有内存,不负责生命周期管理
·几乎零开销,可按值传递 */


/* fmfjnhadmnQPWAN HDQWMDQW HDFQWMJDS BDQKWSH 9U

string_view:
const char* buffer_ptr = address of "H"  HDQWMDQW
int length = 8 
*/

#include <string_view>
#include <string>
#include <iostream>

//移除当个前缀
inline std::string remove_one_prefix(const std::string& s)
{
    return s.substr(1);
}

inline std::string_view remove_one_prefix_view(const std::string& s)
{
    return s.substr(1);
}


/* int main(){
    std::string_view sv  = "hello , c++";
    std::string string_source  =  "hello , this is" ;
    std::string_view string_source_view (string_source);
    const char buf[]  = {'a' , 'b'};
    const std::string_view sv3(buf , 2);

    auto fruit  = std::string {"apple"};
    auto new_fruit = remove_one_prefix(fruit);
    std::cout<< new_fruit << "\n" ;

    std::string_view sv = std::string ("temp");  // temp为临时对象，随时会被回收，不一定只在函数结束时回收
    std::string owned_sv(sv) ;

}
 */
