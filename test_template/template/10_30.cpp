#include <string>
#include <cstring>

template <typename T>
void Tsum(T &a,const T &b)
{
    a+=b;
}

//特例
template<>
void Tsum<char *>(char* &a,  char* const &b)
{
    std::strcat(a,b);
}

struct Test{
    int a,b;
};

template<>
void Tsum<Test>(Test &a, const Test &b)
{
    a.a += b.a;
    a.b += b.b;
}

void sum(int &a,int b)
{
    a+=b;
}

void sum(std::string &a,const std::string &b)
{
    a +=b;
}

void sum(char *a, const char *b)
{
    std::strcat(a,b);
}


struct T1{
   constexpr static int num=1;
};


struct T2{
  constexpr static int num=2;
};

//如果要获取这两个类中的值
// T1::num
// T2::num

template <typename T>
constexpr static int num =T::num;

//获取T1和T2中的num
int a= num<T2>;


void Demo()
{
    int n1=1;
    Tsum(n1,2);
    double d1=5.4;
    Tsum(d1,6.8);

    std::string s1="hello";
    Tsum(s1,std::string("abc"));

    // cstr 是数组类型 char[4]，不能绑定到 char*&（引用到指针）。
    // 数组到指针的衰减不会发生在“引用到指针”的形参上。
    // "aaa" 衰减为 const char*，不能绑定到 char* const&。

    char cstr[] = "bbb";
    //Tsum(cstr,"aaa");
    char cs1[] = "aaa";
    char *p1 =cstr;
    char *p2 =cs1;
    Tsum(p1,p2); //调用特例化版本

    Test t1{1,2};
    Test t2{3,4};

    Tsum(t1,t2);
}


