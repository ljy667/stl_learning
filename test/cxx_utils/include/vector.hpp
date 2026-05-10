
#pragma once

#include <cstddef>


template <class T>
class vector {
public: 
    using value_type = T ;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;  //有符号指针类型
    using reference = value_type&;
    using const_reference = const value_type&;
    using iterator = value_type*;  //用指针当作迭代器类型？
    using const_iterator =const value_type*;
private:
    T* allocate (size_type n)
    {
        T* ptr = static_cast<T*> operator new (n *sizeof(value_type));
        return ptr;
    }

    void deallocate(T* ptr){
        operator delete(p);
    }

    T* m_start;     //指向这段内存开头
    T* m_finish;    //指向元素的结尾
    T* m_end_of_storage;  //指向这段内存结尾

public:
    void show() const
    {
        for(const value_type& val : *this)
        {
            std::cout << val << " ";
        }
        std::cout<< "\n";
        std::cout<< "size = "<< size() << " , capacity = " <<  capacity() << "\n" ;
    }

    size_type size() const
    {
        return m_finish - m_start ;
    }

    size_type capacity() const
    {
        return m_end_of_storage - m_start;
    }

    iteraotr begin(){
        return m_start;
    }

    iteraotr end(){
        return m_finish;
    }
} 