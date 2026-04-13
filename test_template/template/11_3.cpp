#include <iostream>
#include <string>
#include <cmath>


template<typename T>
T SumAbs(const T &a)
{
    return std::abs(a);
} //两个数相加

//多个数相加
//变参的三种展开方式
//1.直接展开（用逗号）
//2.嵌套展开（先嵌套，然后再用逗号展开）
//3.按照自定义运算符展开（折叠表达式）
template<typename T,typename... Args>
T Sum(const T &a,const Args &... args)
{
    return a+Sum<Args...>(args...); //直接展开，如果传的是（1，2，3），Sum<int,int,int>(1,2,3)
}


//递归模板实例化，生成函数的过程中用到了递归

//Args为一组typename,...表示将其展开
//模板进行实例化时递归，函数调用时不存在递归
//当递归到args只剩一个时，则调用Sum(const T &a)

//嵌套展开
template<typename T,typename... Args>
T SumAbs(const T &a,const Args &... args)
{
    return std::abs(a)+SumAbs(std::abs(args)...); //先嵌套（整个args嵌套在std::abs中），再展开
}

//T Sum(const T &a, Args ... args)

//对于Sum<int,int,int>，第一个int匹配T，后面两个int匹配Args...，
void Demo(){
    std::cout<<SumAbs<int,int,int>(1,2,3)<<std::endl;

    SumAbs<int,int,int,int>(1,2,3,4)->std::abs(1)+SumAbs(std::abs(2),std::abs(3),std::abs(4));

    //具体调用,不含有函数递归
    Sum<int>(3);
    Sum<int,int>(2,3) -> 1+Sum<int>(3);
    Sum<int,int,int>(1,2,3) -> 1+Sum<int,int>(2,3);
    
}




//C++模板元编程之折叠表达式

template<typename...Args>
auto Sum(Args... args)
{
    //a1,a2,a3,a4
    //(a1+a2)+a3)+a4
    //return(...+args); //左折叠表达式
    
    return (... + args); //一元折叠表达式
}

template<typename...Args>
{
    void Print(const Args &... args)
    {
        (std::cout<<... << args) << std::endl; //二元折叠表达式（额外东西参与）
    }
}


