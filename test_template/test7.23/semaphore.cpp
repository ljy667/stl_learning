#include <iostream>
#include <thread>
#include<semaphore>
#include <thread>

// A counting_semaphore contains an internal counter initialized by the constructor. 
// This counter is decremented by calls to acquire() and related methods, and is incremented by calls to release(). 
// When the counter is zero, acquire() blocks until the counter is incremented, but try_acquire() does not block; 
// try_acquire_for() and try_acquire_until() block until the counter is incremented or a timeout is reached.


std::binary_semaphore semMaintoThread(0)
,semThreadtoMaint{0};
// {0} 初始值：意味着信号量的初始状态为“未释放”或“不可用”。
// 也就是说，任何试图等待（acquire()）这个信号量的线程都会在最开始阻塞，直到有人释放（release()）它。

void threadProc()
{
    std::cout << "Thread started, waiting for main thread to signal...\n";
    // 等待主线程信号
    semMaintoThread.acquire();
    std::cout << "Thread received signal from main thread.\n";
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1s);
    // 发送信号给主线程
    semThreadtoMaint.release();
    std::cout << "Thread finished execution.\n";
}


//初始值为3
std::counting_semaphore<3> sem(3);



// int main()
// {
//     std::thread th(threadProc);
//     std::cout << "Main thread is doing some work...\n";
//     semMaintoThread.release(); // 发送信号给子线
//     semThreadtoMaint.acquire(); // 等待子线信号
//     std::cout << "Main thread received signal from child thread.\n";
//     th.join(); // 等待子线结束
// }


int main(void)
{
    const int a=8;
    // &a的类型实际上是const int*，
    // 因为a是const，不能用普通的int*指向，除非进行强制转换。
    int *pa =(int *)&a;
    *pa=4;
    std::cout<<*pa<<a;
    return 0;
}