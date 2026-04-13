#include<iostream>
#include<string>
#include<future>
using namespace std;

int  getsum(int a,int b) {
	int vl = a+b;
	for (int i = 0; i < 10000; i++)
	{
		vl++;
	}
	return vl;
}


/// <summary>
///  使用promise
/// </summary>
/// <param name="int_promise"></param>
void async_task(std::promise<int> int_promise) {
	// 模拟一些工作  
	std::this_thread::sleep_for(std::chrono::seconds(1));

	// 存储结果  
	int_promise.set_value(42);
}

void async_task1()
{

}

int main() {

	//第一种玩法：按照future+async的方式开辟一个新的线程
	future<int> myyibu = async(launch::async,getsum,20,30);
	cout << "this is yibu" << myyibu.get() << endl;
	cout << "xia mian shi tongbu" << endl;
	cout <<"zhijiediaoyong:"<< getsum(10,20) << endl;

	//第二种玩法：按照package_task的方式进行异步调用
	packaged_task<decltype(getsum)> mytsak(getsum); // 也可以按照写法来调用，使用decltype 获得函数的定义类型
	//packaged_task<int(int,int)> mytsak(getsum); //这种是原始的方法
	mytsak(30, 30);
	auto myf = mytsak.get_future().get();
	cout << "task result" << myf << endl;


	//第三种：promise
	promise<int> mypromise; //创建promise对象
	future<int> myfuture3 = mypromise.get_future(); // 从promise对象获取future对象  
	std::thread t(async_task, std::move(mypromise)); // 在新线程中执行异步任务 
	// 在主线程中等待并检索结果  
	int value = myfuture3.get();  // 这里会阻塞，直到async_task调用set_value()  
	// 输出结果  
	std::cout << "Value received: " << value << std::endl;
	// 等待线程结束  
	t.join();
	/*在将std::promise对象传递给线程时，我们使用std::move来移动它，因为std::promise是不可复制的。
		调用std::future::get()会阻塞，直到相应的std::promise对象调用set_value()或set_exception()。
		你可以使用std::future::wait_for()或std::future::wait_until()来等待一段时间，而不是无限期地等待。
		当std::future对象被销毁并且它的值还没有被检索时，如果std::promise对象还没有被销毁，则存储的值会被丢弃。
		如果std::promise对象先被销毁，则尝试从std::future对象检索值会导致std::future_error异常。*/


	return 0;
}



