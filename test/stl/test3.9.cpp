#include <cstdlib> // std::malloc/std::free
#include <stdexcept> // std::runtime_error
#include <vector>
#include <iostream>
// 自定义 Stack 类型（带动态资源）
struct Stack {
    int size;
    int* buf; // 动态分配的缓冲区（模拟资源）

    Stack(int s) : size(s), buf(new int[s]) {
        if (size <= 0) throw std::invalid_argument("size must be positive");
    }

    ~Stack() {
        delete[] buf; // 析构时释放资源
        buf = nullptr;
        size = 0;
    }
};

// 修正后的 demo 函数
void demo() {
    // 1. 分配原始内存，检查是否成功
    void* malloc_ptr = std::malloc(sizeof(Stack));
    if (malloc_ptr == nullptr) {
        throw std::runtime_error("malloc failed");
    }

    // 2. 第一次构造对象 → 析构对象（内存保留）
    Stack* s = new(malloc_ptr) Stack(4); 
    s->~Stack(); // 析构对象，释放 Stack 内部的 buf 资源

    // 3. （可选）重新构造对象 → 析构对象（确保构造前内存无对象）
    Stack* s2 = new(malloc_ptr) Stack(8); 
    s2->~Stack(); // 析构第二个对象，释放内部 buf 资源

    // 4. 释放原始内存（必须在所有对象析构后）
    std::free(malloc_ptr);
    malloc_ptr = nullptr; // 置空，避免野指针
}

int binarySearch(std::vector<int> &num, int target )
{   
    //索引
    int i =0 , j= num.size()-1;
    while(i<=j)
    {
        int m = i+(j-i)/2 ;
        if(num[m]<target)
        {
            i=m+1;
        }
        else if(num[m]>target)
        {
            j=m-1;
        }
        else
            return m;
    }
    return -1;
}

class MyClass{
    public:
        MyClass(){};
        MyClass(const MyClass&) {
            std::cout<<"COPY";
        }
        MyClass(MyClass&&) 
        {
            std::cout <<"MOVE";
        }
};

int main (){
    std::vector<MyClass> vec;
    vec.reserve(2);  //分配2个元素的内存
    vec.push_back(MyClass());
    vec.push_back(MyClass());
 std::cout<<"==================================";
    vec.push_back(MyClass());
    return 0;
    
}
        
Stack& operator=(const Stack& other) {
    if (this == &other) return *this;
    delete[] buf; // 释放旧资源
    buf = new int[other.size]; // 重新分配内存，可能抛bad_alloc
    size = other.size;
    return *this;
}