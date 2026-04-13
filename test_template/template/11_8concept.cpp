


#include <iostream>
#include <type_traits>
#include <utility>
#include <concepts> 

//concept就是bool类型的编译期常量,主要为了做模板中的类型限定

//这个模板只适用于特定类型的T,不一定是限于类型，例如该类型具有一些特定的函数

template<typename T>
constexpr bool SmallType=sizeof(T)<sizeof(void*);


template<typename T>
std::enable_if_t<SmallType<T>,void>
f(T t)
{
    std::cout<<"SmallType"<<std::endl;
}

template<typename T>
requires std::is_trivial_v<T>&&SmallType<T>
void g(T t)
{
    std::cout<<"SmallType"<<std::endl;
}

//c++20, 更好的写法
//============================================================
template<typename T>
concept CouldCopy=requires{
    std::is_trivial_v<T>;   //concept  T 是平凡类型
    SmallType<T>;   //concept  T 是小类型
}

template<typename T>
requires CouldCopy<T>
void e(T t)
{
    std::cout<<"CouldCopy"<<std::endl;
}


template<typename T>
concept NewConc =
requires{
    CouldCopy<T>;   //concept  T 满足 CouldCopy 条件
    std::is_pod_v<T>;    //concept  T 是 POD 类型
};

template<typename T>
requires NewConc<T>
void h(T t)
{
    std::cout<<"NewConc"<<std::endl;
}
//============================================================


//requires 定义一组concept
template<typename T>
requires requires{
    std::is_trivial_v<T>;   //concept  T 是平凡类型
    SmallType<T>;   //concept  T 是小类型
}
void f(T t)
{

}


//报错示例
struct Test
{
    int arr[100];
};

void Demo()
{
    Test t;
    f(Test)(t); //报错，Test类型不满足SmallType条件,
                //并且无largeType版本的f函数
}

//=================================2025 11.9 concept 下 打洞哥==============================
//c++17的写法
template<typename T>
constexpr bool HasMemF = std::is_void_v<std::void_t<decltype(std::declval<T>().f())>>;

template<typename T>
std::enable_if_t<HasMemF<T>, void>
func(T t)
{
    std::cout << "HasMemF" << std::endl;
}

//判断T内是否有f（）
template<typename T>
concept HasMemF_C = requires{
    std::declval<T>().f();
};

template<typename T>
concept HasMemF_C = requires(T t){
    t.f();
};

template<typename T>
constexpr bool HasMemIntF = std::is_same_v<decltype(std::declval<T>().f()), int>;

//如果返回值有限定（判断返回值），c++20语法
// 用concept语法去判断一个表达式的返回值类型

/* std::is_same_v 本身就是一个“模板全局常量”（更准确叫：变量模板）。
它等价于 inline constexpr bool is_same_v<T, U> = is_same<T, U>::value;。
所以：std::is_same_v 与你说的“模板全局常量 constexpr”是同一类机制的一个标准库实例。 */

template<typename T>
concept HasMemIntF = requires(T t){
    { t.f() } -> std::same_as<int>;   // same_as<int,decltype(t.f())>
};                                      //std::same_as是一个concept的封装，如我下面自己写的SameAs
                                    //这里就等价于把t.f()的返回值放到std::is_same_v的第一个参数，并和int做比较，这个int自己填入

template<typename T1,typename T2>
concept SameAs =std::is_same_v<T1,T2>;

template<typename T>
concept HasMemIntF = requires(T t){
    sizeof(T) <16;
    { t.f() } -> std::same_as<int>;   // same_as<int,decltype(t.f())>
};

template<typename T>
requires HasMemIntF<T>
void func_int(T t)
{
    std::cout << "HasMemIntF" << std::endl;
}

//也可以直接，意思也就是说func_int2函数的参数需满足HasMemIntF概念的类型t，HasMemIntF修饰auto这个类型
//虽然可以不写模板头，但还是模板
void func_int2(HasMemIntF auto t)
{
    std::cout << "HasMemIntF" << std::endl;
}


template<typename T>
requires HasMemF_C<T>
void func_concept(T t)
{
    std::cout << "HasMemF_C" << std::endl;
}


