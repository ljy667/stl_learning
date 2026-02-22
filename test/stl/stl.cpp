#include <vector>
#include <type_traits>
#include <array>
#include <unordered_map>
#include <map>
#include <list>


//适合顺序结构，非链式结构
template<typename T>
struct Alloc
{
    using value_type= T ; 

    Alloc();
    ~Alloc();
    T* allocate(size_t size);
    void deallocate(void *ptr, size_t size);

    template<typename U>
    struct rebind{
        using other =Alloc<U>;
    };

    private:
    void* buffer_;
    std::unordered_map<void* ,size_t> table_2; //登记

    //每个位置之间分配的空间的大小,ordered
    std::map<size_t,size_t> table_;
    //维护一个map, table_->first是所需size大小内存块的起始字节位序，table_->second是所需size大小内存块的大小
};

//看buffer中哪一个空隙是够一个size的，分配出去，并登记
template<typename T>
T* Alloc<T>::allocate(size_t size)
{   
    auto last_iter=table_.begin();

    //结构化绑定 auto [pos,sec_size]:table_：C++17 特性，简化遍历 map 的写法
    //for(auto [pos,sec_size]:table_){}

    for(auto iter =table_.begin();iter!=table_.end();++iter) 
    {
        if(iter==table_.begin()){continue;}

        if(iter->first-last_iter->first>=last_iter->second+size){
            table_[last_iter->first+last_iter->second]=size; //下一个size大小内存起始位置的登记
            return reinterpret_cast<T*>(static_cast<char*>(buffer_)+last_iter->first+last_iter->second);  
        }
        last_iter=iter;
    }
       //新分配的内存指针
}

template<typename T>
void Alloc<T>::deallocate(void *ptr, size_t size)
{
    //char*占位1个字节，ptr-buffer=>ptr在buffer中的字节位序，即偏移量
    auto iter=table_.find(static_cast<char*>(ptr)-static_cast<char*>(buffer_)); 
    if(iter!=table_.end()){
        if(iter->second==size){
            table_.erase(iter); //删除登记
        }
    }
}

//分配空间，自行管理 
template<typename T>
Alloc<T>::Alloc():buffer_(std::malloc(1024)) //1024字节，1kb
{

}

template<typename T>
Alloc<T>::~Alloc()
{
    std::free(buffer_);
}
 

struct Alloc_int{
    using value_type= int ;
    int* allocate(size_t size){
        return static_cast<int*>(std::malloc(size));
    }
    void deallocate(void *ptr, size_t size){
        std::free(ptr);
    }
    template<typename T>
    struct rebind{
        using other =Alloc<T>
    };
};


//链式  typename alloc = std::allocator<T>代表什么意义
template<typename T,typename alloc = std::allocator<T>>
struct List{
    struct Node
    {
        T value;
        Node* next;

        template<typename... Args>
        Node(Args&&... args):value(std::forward<Args>(args)...),next(nullptr){}
    };
    
    template<typename... Args>
    void PushBack(Args&&... args)
    {
        if(header==nullptr)
        {
            //header=new Node(std::forward<Args>(args)...);
          void* header = alloc_.allocate(sizeof(Node));
          Node* new(header) Node(std::forward<Args>(args)...);
        }
    }

private:
    Node* header =nullptr;
    typename alloc::template rebind<Node>::other alloc_ ; // template、typename的作用 二一性？
};




int main(int argc,const char *argv[])
{
    std::vector<int,Alloc<int>> ve {1,2,3} ;
    std::list<int,Alloc<int>> li(ve.begin(),ve.end());
    //ve.begin()：指向 ve 第一个元素的迭代器（起始位置）；ve.end()：指向 ve 最后一个元素下一个位置的迭代器（结束位置）；

}