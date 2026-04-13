
#include <iostream>
#include  <type_traits>

struct Point
{
    double x,y;
};

struct Compare{
    bool operator()(const Point& l,const Point& r) const noexcept;
};

bool Compare::operator()(const Point& l,const Point& r) const noexcept
{
    return l.x<r.x && l.y<r.y;  //更靠近左下就是true
}

//排序函数
template<typename T>
void Sort(T *arr,size_t size)
{
    for(int i=size;i>1;i--)
    {
        for(int j=1;j<i;j++)
        {
            if(arr[j]<arr[j-1]){  //T必须支持<运算符
                T tmp=arr[j];
                arr[j]=arr[j-1];
                arr[j-1]=tmp;//调换位置
            }
        }
    }
}

void Demo()
{
    Point p1(2,5);
    Point p2(1,2);
    Compare com;
    std::cout<<com(p1,p2)<<std::endl;
    //为何使用仿函数，为何不直接使用函数
}

/* 
在你使用复制元素的算法时，shared_ptr 在 C++ 标准库容器中很有用。 
您可以将元素包装在 shared_ptr 中，然后将其复制到其他容器中（请记住，只要您需要，基础内存就会一直有效）。
以下示例演示如何在向量中对 remove_copy_if 实例使用 shared_ptr 算法。
 */

//三个独立对象 + 三个独立控制块
std::vector<std::shared_ptr<Song>> v {
  std::make_shared<Song>(L"Bob Dylan", L"The Times They Are A Changing"),
  std::make_shared<Song>(L"Aretha Franklin", L"Bridge Over Troubled Water"),
  std::make_shared<Song>(L"Thalía", L"Entre El Mar y Una Estrella")
};

//这里判断谓词lambda 里的参数 s 来自输入容器 v 的每一个元素

std::vector<std::shared_ptr<Song>> v2;
remove_copy_if(v.begin(), v.end(), back_inserter(v2), [] (std::shared_ptr<Song> s) 
{
    return s->artist.compare(L"Bob Dylan") == 0;
});

for (const auto& s : v2)
{
    std::wcout << s->artist << L":" << s->title << std::endl;
}


struct X{
    int f() const noexcept{
        return 58;
    }
    void g() noexcept{

    }
};
int foo() noexcept{
    return 42;
} 

//根据模板参数T确定函数是否抛出异常
template<typename T>
T copy(const T& t)
 noexcept(std::is_fundamental<T>::value)
 //可根据函数是否是基础类型来决定是否抛出异常
 {
    return t;
 }

 int foo() noexcept{
    return 42;
 }

 int foo1()
 {
    return 42;
 }

 int foo2() throw()
 {
    return 42;
 }

 int main()
 {
    std::cout<<std::boolalpha;
    std::cout<<"foo() noexcept:"<<noexcept(foo())<<std::endl; //true
    std::cout<<"foo1() noexcept:"<<noexcept(foo1())<<std::endl; //false
    std::cout<<"foo2() noexcept:"<<noexcept(foo2())<<std::endl; //true
 }


 //std::integral_constant<bool, 条件> 把“条件结果”封装成一个不同的类型，用于编译期重载选择。
 /* 条件可以是任何可在编译期计算为 true/false 的表达式————编译期常量（traits、sizeof、Alignment、常量表达式逻辑组合等）。
    这样就不需要手写 true/false；让类型系统自动帮你挑对应重载。 */

    //再配合noexcept使用，可声明函数绝不抛异常
    //例如
    template<typename T>
    void swap_impl(T& a,T& b) noexcept
    {
    swap_impl(a, b, std::integral_constant<bool,
    std::is_nothrow_move_constructible_v<T> &&
    std::is_nothrow_move_assignable_v<T>
        >{});
    }


 template<typename T>
 void swap_impl(T& a,T& b,std::integral_constant<bool,true>) noexcept
 {
    T tmp=std::move(a);
    a=std::move(b);
    b=std::move(tmp);
 }

  template<typename T>
 void swap_impl(T& a,T& b,std::integral_constant<bool,false>) 
 {
    T tmp(a);
    a=b;
    b=tmp;
 }




 