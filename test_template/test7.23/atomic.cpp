#include <iostream>
#include <thread>
#include <vector>
#include <atomic>

std::atomic<int> counter(0);

void increament(){
    for(int i=0;i<10000;i++)
    {
        //  counter.fetch_add(1); 
        // 原子操作,保证每次递增都不会被中断，不会丢失中间的数值。
        counter++;
    }
}

int main(){
    std::vector<std::thread> threads;
    for (int i=0;i<10;i++)
    {
        threads.emplace_back(increament);
    }
    for(auto& t:threads)
    {
        t.join();
    }
    //load()方法获取当前值。
    std::cout<< "final counter value:"<<counter.load()<<std::endl;
    return 0;
}