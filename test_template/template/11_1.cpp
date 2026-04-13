
#include <iostream>
#include <string>
template <typename T,size_t N>
class Array{
    public:
    Array();
    Array(int n , T arg);
    
    T &at(int n);
    T &operator [](int n);

    private:
    T data[N];

};

//针对于模板类Array作函数实现
template <typename T,size_t N>
Array<T,N>::Array(int n , T arg)
{
    for(int i=0;i<n;i++)
    {
        data[i]=arg;
    }
}

template <typename T,size_t N>
T &Array<T,N>::at(int n)
{
    return data[n];
}

template <typename T,size_t N>
T &Array<T,N>::operator [](int n)
{
    return this->at(n);
}

void Demo()
{
    //前三个元素，赋值8
    Array<int,5> arr(3,8);
    std::cout<< arr[4]<<std::endl;
    arr[8] =10;
    std::cout<<arr.at(8)<<std::endl;
}

//类型萃取
//对于int，double char，指针类型T*->T
//对于其他类型， Txxx ->const T&

//因此需要类型萃取类，或者说是类型处理类
//模板特例
template<typename T>
struct ArgType
{
    //std::string ,则const std::string &
    using type = const T &;
};

//偏特化
template<typename T>
struct ArgType<T*>
{
    using type = T*;
};


//以下为全特化 (模板类型匹配)
template<>
struct ArgType<int>
{
    using type =int ;
};

template<>
struct ArgType<double>
{
    using type = double;
};

template<>
struct ArgType<char>
{
    using type = char;
};

//模板类型定义
template<typename T>
using ArgType_t = typename ArgType<T>::type ;

//对于指针，直接传指针，不传指针引用
template<typename T>
void Show(ArgType_t<T> arg)  //这里的 ArgType<T>::type  就是类型，而不是变量
{
    std::cout<<arg<<std::endl;
}



void Demo()
{
    Show<int>(5);
    std::string str= "123" ;
    std::string* p=&str;
    Show<std::string>(str);  //传引用,而不是复制
    Show<std::string *> (p);
}





