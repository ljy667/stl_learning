#include <iostream>
#include <thread>
#include <barrier>
#include <vector>

void worker(int id, std::barrier<>& sync_point)
{
    std::cout << "Worker " << id << " waiting at barrier." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100 * id  ));

    sync_point.arrive_and_wait();
    std::cout << "Worker " << id << " passed the barrier." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100 * id  ));
}

int main()
{
    const int num_threads =5;
    std::barrier sync_point (num_threads);

    std::vector<std::thread> threads;
    for(int i=0;i<num_threads;i++)
    {
       // 创建线程并传入栅栏对象，所有线程都用栅栏
         threads.emplace_back(worker, i+1, std::ref(sync_point));
    }

    for(auto& t:threads)
    {
        t.join();
    }
    
}



