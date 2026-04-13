

//内存分配器

#include <iostream>
#include <string>
#include <cmath>
#include <type_traits>

uint8_t buf[2048];
uint8_t *pos=buf;   //8位，一个字节

//返回当前游标 pos 作为已分配块起始地址
//然后将 pos 向前推进 size 字节（立即推进，而非使用后再推进）
//就是把这个pos指针指向的内存向右移动size大小
void *Alloc(size_t size)
{
    auto tmp=pos;
    pos+=size;
    return tmp;
}

class Point
{
    public:
        Point(double x,double y);
        Point(const Point &)=delete; //禁用拷贝构造函数
        void show() const;
    private:
        double x_,y_;
};

//定义
Point::Point(double x,double y):x_(x),y_(y){}
void Point::show() const
{
    std::cout<<"("<<x_<<","<<y_<<")"<<std::endl;
}   

struct Test{
    Test(Point &p);
};


class Circle{
    public:
    Circle(const Point &center,double r);
};

/* //变参模板 ... Args，支持任意类型
//创建第一个类型为 T 的对象，传入的参数包 args... 用于调用 T 的构造函数
template<typename T,typename... Args>
T *Create(Args... args)   //传入参数包-> Circle *create(Point arg) 这里会产生值传递，
                            //1值传递有可能会遇到不可复制类型，也有可能会遇到复制开销大的类型
                            //例如我禁用 Point的拷贝构造函数
{
    T* buf=static_cast<T *>(Alloc(sizeof(T)));
    return new(buf) T(args...);  //placement new 
                                 //args...里面的参数参数用逗号隔开
} */

//避免值传递
template<typename T,typename... Args>
T *Create(const Args&... args)   
{
    T* buf=static_cast<T *>(Alloc(sizeof(T)));
    return new(buf) T(args...); 
}




//引用折叠
template<typename T,typename... Args>
T *Create_sub(Args&&... args)
{
    T* buf =static_cast<T *>(Alloc(sizeof(T)));
    return new(buf) T(Forward<Args>(args)...);    
}


//Forward函数实现完美转发,两个重载
template<typename T>
T &&Forward(T &t)
{
    return static_cast<T &&>(t);
}


template<typename T>
T &&Forward(T &&t)
{
    return static_cast<T &&>(t);
}


void Demo()
{
    int *data=static_cast<int *>(Alloc(sizeof(int))); 
    *data =18;

    int *data_1=Create<int,int>(18);

    Point *p=Create<Point,double,double>(1,5.6);
    p->show();

    Circle *c =Create<Circle,Point,double>(*p,3.5);

    //分配内存空间
    Point *p=static_cast<Point *>(Alloc(sizeof(Point)));
    //placement new方法，手动调用构造函数，在分配的内存上构造对象
    new (p) Point(1.0,2.0);
    p->show();


    Point p2(1,2);
    //这里把第二个模板参数设置为Point&类型的原因是Test的构造函数需要Point &类型参数
    Test *t=Create<Test,Point&>(p2);
    //根据T *Create(const Args&... args)，p2会以const & 引用形式传入
    // 但是根据Test的构造,Test(Point &p); 非const引用无法绑定到const对象上

    //引用折叠
    Point &&rp=std::move(p2); //p2是左值，std::move后变成右值
    Test *t=Create_sub<Test,Point&>(rp); //rp是右值，传入右值引用

    auto &&res1= Forward<int>(5);
    int a=5;
    int &res2= Forward<int &>(a); //a是左值，传入左值引用
    /* 
    模板展开
    int &Forward(int &t)
    {
        return static_cast<int &>(t);
    }
    */

    int&& rr=std::move(a);
    auto &&res3=Forward<int &&>(rr);

    //传的是右值引用，右值引用本身是左值，所以会传入以下这个重载
    /* int &&Forward(int &t)
    {
        return static_cast<int &&>(t);
    } */
}
