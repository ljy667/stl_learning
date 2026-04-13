#include <thread_test.h>


int main(void)
{
    const int N_STEPS =10000000;
    std::thread th1;
    std::thread th2;
    thread_test tester;

   {
        std::promise<double> promise;
        auto receiver=promise.get_future();
        //th1=std::thread(&thread_test::compute_pi,&tester,N_STEPS,std::move(promise));

        th1=std::thread([&tester, promise=std::move(promise)]()mutable{
            tester.compute_pi(N_STEPS, std::move(promise));
        });

        th2=std::thread([&tester, receiver = std::move(receiver)]() mutable {
            tester.display(std::move(receiver));
        });

    }
    th1.join();
    th2.join();

    return 0;
}