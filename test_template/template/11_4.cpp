
//类型限定


#include <iostream>
#include <string>
#include <cmath>
#include <type_traits>
#include <utility>

struct Test{
    //void method(){}
    int method();
};

struct Test2{

};

template<typename T1,typename T2>
struct  IsSame
{
    constexpr static bool value = false;
};

template<typename T>
struct IsSame<T,T>
{
    constexpr static bool value = true;
};

template<typename T1,typename T2>
constexpr bool IsSame_v = IsSame<T1,T2>::value;

template <typename T>
struct ToInt
{
    using type = int;
};
//同理，这个int 可改成其他类型，目的就是统一类型
//因此因使用最简单的类型来做类型转换的中间媒介
template <typename T>
struct X{
    using type =void;
};

//因此下面的f函数可改为
template<typename T>
std::enable_if_t<std::is_same_v<typename X<decltype(std::declval<T>().method())>::type,void>,void>
f(const T &t)
{
    std::cout<<2<<std::endl;
}

//std::is_void_v 判断传入类型是否为void类型
//stl又一个工具 std::void_t,不管传入一切是什么类型，都传出void，即我们自己写的X模板类
template<typename T>
std::enable_if_t<std::is_void_v<typename X<decltype(std::declval<T>().method())>::type>,void>
f(const T &t)
{
    std::cout<<2<<std::endl;
}



template<typename T>
//终极写法，要求T必须实现method()函数，且返回值无所谓
std::enable_if_t<std::is_void_v<std::void_t<decltype(std::remove_reference_t<T>().method())>>,void>
f(const T &t)
{
    std::cout<<2<<std::endl;
}


//要求T的SIZE小于指针,此要求较简单
//std::enable_if_t<(sizeof(T)< sizeof(void*)),void>

//对传入类型更加复杂的要求，例如要求T必须实现 method（）函数
// 构造一个T类型的对象，调用method（），如果可以正常调用，我们就判断返回值是不是void，不可正常调用的就不能匹配

template<typename T>
std::enable_if_t<std::is_same_v<decltype(T().method()), void>, void>
//使用static_cast构造一个T类型的指针,无需满足是否具有默认构造函数
std::enable_if_t<std::is_same_v<decltype(static_cast< T* >(nullptr)->method()),void>,void>

//针对此种情况，Stl实现了一个专门的方式std::declval<>().
std::enable_if_t<std::is_same_v<decltype(std::declval<T>().method()),void>,void>


//2若要求T必须实现method()函数，但返回值无所谓
template<typename T>
std::enable_if_t<std::is_same_v<typename ToInt<decltype(std::declval<T>().method())>::type,int>,void>
f(const T &t)
{
    std::cout<<2<<std::endl;
}

fui(const T &t)
{
    std::cout<<1<<std::endl;
}

void Demo()
{
    Test t;
    Test2 t2;
    fui<Test>(t);
    fui<Test2>(t2);

}