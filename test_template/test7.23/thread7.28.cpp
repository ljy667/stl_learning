#include <iostream>
#include <thread>
#include <condition_variable>

std::mutex cv_mtx;
std::condition_variable cv;
bool ready= false;

void print_id(int id)
{
    std::unique_lock<std::mutex> lock(cv_mtx);
    //cv.wait() 需要一个 std::unique_lock<std::mutex> 对象作为参数
    cv.wait(lock,[]{return ready;});
    std::cout << "Thread " << id << " is printing" << std::endl;
}

void  set_ready(){
    std::this_thread::sleep_for(std::chrono::seconds(1));
    {
        std::lock_guard<std::mutex> lock(cv_mtx);
        ready= true;
    }
    cv.notify_all();
}

int main()
{
    std::thread t1(print_id,1);
    std::thread t2(print_id,2);

    std::thread t3(set_ready);

    t1.join();
    t2.join();
    t3.join();

    return 0;
}