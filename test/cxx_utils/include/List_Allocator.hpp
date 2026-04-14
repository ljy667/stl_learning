#pragma once
#include <cstddef>

template<class T>
class MyAllocator{

    public:
    //定义别名
    using value_type = T;
    using size_type = std::size_t ;
    //std::ptrdiff_t:存储两个同类型指针相减的结果
    using difference_type = std::ptrdiff_t;
    using pointer = value_type*;

    
    //1.分配
    //Stack* s = new(mem) Stack(4);
    static pointer allocate(size_type count)
    {
        auto s =  (pointer)operator new (count * sizeof(value_type)) ;
        
        return s ;

    }

    //2.释放

    static void deallocate(pointer p)
    {   
        operator delete (p);
    }

};