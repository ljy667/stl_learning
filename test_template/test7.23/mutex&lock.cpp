 #include <mutex>
 #include <iostream>
 #include <thread>
 class Counter{
    public:

    void inc()
    {
        counter_mutex.lock();
        m_count++; //临界区，多个线程或进程会同时访问
        counter_mutex.unlock();
    }

    int get()
    {   int temp;
        counter_mutex.lock();
        temp =m_count;
        counter_mutex.unlock();
        return temp;
    }
    private:
     std::mutex counter_mutex; 
     int m_count =0;
 };

 //lock_guard
  class Counter_lock{
    public:
    void inc(){
        std::lock_guard lock(counter_mutex);
        m_count++;
    }
    int get(){
        std::lock_guard lock(counter_mutex);
        return m_count;
    }

    private:
    std::mutex counter_mutex;
    int m_count =0 ;
  };


 int main(void)
 {
    Counter counter;
    auto increase = [](Counter& counter){for(int i=0;i<10000000;i++) counter.inc();};
    std::thread th1(increase,std::ref(counter));
    std::thread th2(increase,std::ref(counter));
    th1.join();
    th2.join();

    std::cout<<"Counter value: "<<counter.get()<<std::endl;
 }