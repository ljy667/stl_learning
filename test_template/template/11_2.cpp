#include <iostream>
#include <string>

struct Test
{
    int arr[999];
};

template<bool Cond,typename T>
struct enable_if
{};

//这里就是一个偏特化（目前只特化了Cond true），只有传进来一个实际的类型时，enable_if<>才会成为一个实际的类
template<typename T>
struct enable_if<true,T>
{
    using type=T;
};

//模板类型重命名，避免使用 ::type时的冗长
template<bool Cond, typename T>
using enable_if_t = typename enable_if<Cond,T>::type ;

template<typename T>
struct IsLessThanPtr
{
    //编译器常量
    constexpr static bool value = (sizeof(T)<=sizeof(void*));
};

template<typename T>
struct NOT
{
    //constexpr static bool value = (sizeof(T)>sizeof(void*));
    //这个NOT模板类用于嵌套模板类IsLessThanPtr类
    constexpr static bool value = !T::value;
};

    //模板全局常量
    template<typename T>
    constexpr bool Not_v=NOT<T>::value;

    template<typename T>
    constexpr static bool IsLessThanPtr_v = IsLessThanPtr<T>::value;
    //SFINAE
    //Substitution Failure Is Not An Error
    //编译器在模板实例化时，如果遇到匹配失败（无法按此方法实例化），不会马上报错，而是会继续匹配其他模板实现

    //如果f函数“(sizeof(T)<=sizeof(void*))” 匹配true失败（Substitution failure），则不会优先使用无type成员的默认 enable_if
    //而会继续匹配其他特化
template<typename T>
    //void f(typename enable_if<(sizeof(T)<=sizeof(void*)),T>::type t)  //void*为8个字节
void f(enable_if_t<IsLessThanPtr_v<T>, T> t)
{
    std::cout<<1<<std::endl;
}

template<typename T>
    //这里typename enable_if<... , T>::type const & 与 const typename enable_if<... , T>::type & 等价。
    //选择前者是惯用法：把依赖名 typename enable_if<... , T>::type 保持成一个整体，然后在外侧加 const &，更不容易误读。

void f(enable_if_t<Not_v<IsLessThanPtr<T>>, T> const &t)
{
    std::cout<<2<<std::endl;
}

void Demo()
{   
    Test t;
    f<Test>(t) ;
    f<int>(1);
}

int main()
{
    Demo();
    return 0;
}   