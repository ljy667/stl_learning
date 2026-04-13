#include <thread>
#include <iostream>
#include <future>
#include <string>
#include <algorithm>
#include <numeric>
#include <mutex>
#include <vector>
class MyClass {
public:
    void memberFunc(std::promise<double> &promise) {
        std::cout << "Member function called." << std::endl;
    }
    void memberFunc(std::promise<double> &&promise) {
        std::cout << "Member function called." << std::endl;
    }

    void memberFunc2(std::future<double> &&receiver)
    {
        std::cout << "Member function2 called." << std::endl;
    }
};

int main() {
   MyClass obj; 
   std::promise<double> promise;
   auto receiver =promise.get_future();
    // 绑定到对象对象

    // 重载函数存在歧义
    // std::thread t(&MyClass::memberFunc, &obj,std::move(promise));
     // 注意：第一个参数是成员函数指针，第二个参数是对象或对象指针
   std::thread t([&obj,promise=std::move(promise)]()mutable
   {obj.memberFunc(std::move(promise));
   });

    std::thread t1(&MyClass::memberFunc2,&obj,std::move(receiver));
    // std::thread t([&obj,]()
    // {obj.memberFunc();
    // });
    
    t.join();
    t1.join();
    return 0;
}

std::mutex m;
struct X
{
    void foo(int i,const std::string& str)
    {
        std::lock_guard<std::mutex> lk(m);
        std::cout <<str<<' '<<i<<'\n';
    }

    void bar(const std::string& str)
    {
        std::lock_guard<std::mutex> lk(m);
        std::cout <<str<<'\n';
    }
    int operator()(int i)
    {
        std::lock_guard<std::mutex> lk(m);
        std::cout << i <<'\n';
        return i+10;
    }
};

template<typename RandomIt>
int paraller_sum(RandomIt beg, RandomIt end)
{
    auto len =end-beg;
    if(len<1000)
    return std::accumulate(beg,end,0);
    RandomIt mid=beg+len/2;
    auto handle=std::async(std::launch::async,
           paraller_sum<RandomIt>,mid,end);
    
    int sum=paraller_sum(beg,mid);
    return sum+handle.get();
}

// int main()
// {
//     std::vector<int> v(10000,1);
//     std::cout << "THE SUM IS" <<paraller_sum(v.begin(),v.end())<<'\n';
//     X x;
//     //Calls (&x)->foo(42, "Hello") with default policy:
//     // may print "Hello 42" concurrently or defer execution
//     auto a1 =std::async(&X::foo,&x,42,"hello");

//     auto a2 =std::async(std::launch::async,&X::bar,x,"world");

//     auto a3=std::async(std::launch::async,X(),43);
//     a2.wait();
//     std::cout<<a3.get()<<'\n';
    
// }


