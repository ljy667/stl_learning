
#pragma once

#include <cstddef>

namespace cxx{
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
            T* ptr = static_cast<T*> (operator new (n *sizeof(T)));
            return ptr;
        }

        void deallocate(T* ptr){
            operator delete(ptr);
        }


        T* m_start;     //指向这段内存开头
        T* m_finish;    //指向元素的结尾
        T* m_end_of_storage;  //指向这段内存结尾 ,结束位置 > m_finish

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

        iterator begin(){
            return m_start;
        }

        iterator end(){
            return m_finish;
        }


        iterator begin() const{
            return m_start;
        }

        iterator end() const{
            return m_finish;
        }

        vector() : m_start(nullptr) , m_finish(nullptr) , m_end_of_storage(nullptr){}

        //把vector初始化为含有n个value_type的容器
        vector(size_type n , const value_type& val = value_type())
        {   
            //分配内存
            m_start  = allocate(n) ;
            //利用标准库构造对象： std::unintialize_fill_n 调用val的拷贝构造
            //std::fill_n 调用拷贝赋值
            std::unintialize_fill_n(m_start , n , val);
            m_finish = m_end_of_storage = m_start + n;
        }

        //基于两个迭代器构造

        template <class InpuIt>
        

        ~vector()
        {
            std::destroy(begin() , end());  //析构资源
            deallocate(m_start);            //释放内存
        }

    };
}