#include <thread_pool.h>


//线程池中每个工作线程的执行函数
void ThreadPool::worker()
{
    while (true)
    {
        //每次循环都定义一个 task,表无参数无返回值的可调用对象。
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(mtx);
            //cv会阻塞当前线程直到isstop ==true或myque 不为空
            cv.wait(lock,[this]{return this->isstop||!this->myque.empty();});
            if (this->isstop &&this->myque.empty())
            {
                return; //线程结束
            }
            task =std::move(this->myque.front()); //访问队头（第一个元素）的引用
            this->myque.pop(); //把队头的元素（任务）从队列中移除
        }// 离开作用域，自动释放锁
        task(); //执行task
    }
}

ThreadPool::ThreadPool(int threadnums):isstop(false)
{
    for (size_t i=0;i<threadnums;i++)
    {
        workers.emplace_back([this](){this->worker();});
    }
}

ThreadPool::~ThreadPool()
{
    {
    std::unique_lock<std::mutex> (mtx);
    isstop=true;}

   //通知所有正在阻塞等待任务的工作线程。(正在 wait())
   //这样线程会被唤醒,让它们醒来检测 isstop 标志，检测到 isstop == true 后，知道线程池要关闭。
    cv.notify_all();

    for(std::thread& onethred:workers)
    {
        onethred.join();
    }
}

//结合前面的std::queue<std::function<void()>> myque;
//任务队列里只能放无参数、无返回值的可调用对象（void() 类型）。

template<typename F,typename ...Arg>
auto ThreadPool::enques(F&& f,Arg&&...args)->std::future<typename std::result_of<F(Arg...)>::type>
{
    using functype =typename std::result_of<F(Arg...)>::type;
    
    //functype()是F(Arg...)的类型声明
    //functype()：说明这个 packaged_task 代表一个不带参数、返回 functype 的可调用对象。
  
    /* 1. 外层 std::make_shared<std::packaged_task<functype()>>
    这个是创建一个指向 std::packaged_task<functype()> 的 std::shared_ptr。
    functype() 表示这个任务是一个无参、返回 functype 类型的可调用对象。
    make_shared 会调用 packaged_task 的构造函数，把参数传进去。 */

    // std::bind是为了把一个“可调用对象 + 它的参数”绑定成一个，变成无参数的可调用对象，
    //  以便放进 std::packaged_task<functype()> 里执行。
    
    auto task=std::make_shared<std::packaged_task<functype()>>(
    std::bind(std::forward<F>(f),std::forward<Arg> (args))
     //这个std::bind 等价于
    );
    
    //获得future
    std::future<functype> rsfuture = task->get_future(); 
    
    //将任务添加到队列
    {
        std::lock_guard<std::mutex> lockguard(this->mtx);
        if(this->isstop)
        throw std::runtime_error("出错：线程池已经停止");

        //将任务添加到队列
        // task 自己不是 void() 类型，而是 functype() 类型（有返回值）。
        //functype() 是函数签名， 类型为  : 返回值为functype，无参数
        // 队列里只能放 void() 类型的任务。

        //emplace 会直接在队列末尾构造这个 lambda（避免多余拷贝）。
        //myque类型是std::queue<std::function<void()>>
        myque.emplace([task](){*task()});
    }
    //一旦 notify_one() 被调用，就会唤醒一个等待的线程，让它去检查队列并执行任务。
    //“我刚刚往队列里塞了一个任务，现在告诉其中一个空闲线程去干活。”
    cv.notify_one;
    return rsfuture;
}


int main() {
	ThreadPool mypool(4);
	for (size_t i = 0; i < 20; i++)
	{
       /*  这一步：

        std::bind 把 函数+参数 绑定成一个无参的可调用对象。
        再用 packaged_task 包装，变成可以获取返回值的任务。
        最后把任务塞进 myque 队列。 */
		auto rsfuture0 = mypool.enques([](int a, int b)->int {
			std::cout << "当先线程：" << std::this_thread::get_id() << std::endl;
		return a + b; }, 10*i, 10*i);
		std::cout << "thread rs:" << rsfuture0.get() << std::endl;

	}
	
	return 0;
}

