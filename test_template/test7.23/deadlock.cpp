#include<iostream>
#include <thread>
#include <mutex>

std::mutex mutex1;
std::mutex mutex2;

void task1()
{
    std::lock(mutex1,mutex2);
    std::lock_guard<std::mutex> lock1(mutex1,std::adopt_lock);
    std::lock_guard<std::mutex> lock2(mutex2,std::adopt_lock);
    std::cout<< "task 1 finished\n";
}

void task2()
{
    std::lock(mutex1,mutex2);
    std::lock_guard<std::mutex> lock1(mutex1,std::adopt_lock);
    std::lock_guard<std::mutex> lock2(mutex2,std::adopt_lock);
    std::cout<< "task 2 finished\n";
}

// int main()
// {
//     std::thread t1(task1);
//     std::thread t2(task2);
//     t1.join();
//     t2.join();
//     return 0;
// }

struct bank_account
{
    explicit bank_account(int balance):balance(balance) {}
    int balance;
    std::mutex m;
};

void transfer(bank_account& from,bank_account& to ,int amount)
{
  if(&from==&to)
  {
    return;
  }

  std::lock(from.m,to.m);
  std::lock_guard<std::mutex> lock1(from.m,std::adopt_lock);
  std::lock_guard<std::mutex> lock2(to.m,std::adopt_lock);

  from.balance -= amount;
  to.balance +=amount;
}

int main(){
    bank_account my_account{100};
    bank_account your_account{50};
    std::thread t1(transfer,std::ref(my_account),std::ref(your_account),50);
    std::thread t2(transfer,std::ref(your_account),std::ref(my_account),20);
    t1.join();
    t2.join();
    std::cout<<"my_account balance: "<<my_account.balance<<std::endl;
    std::cout<<"your_account balance: "<<your_account.balance<<std::endl;
}