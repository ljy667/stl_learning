
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
    vector() 
            : m_start(nullptr) ,m_finish(nullptr) , m_end_of_storage(nullptr) {}


    size_type size() const
    {
        return m_finish - m_start ;
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
        vector(InpuIt first , InpuIt last)
        {
            size_type n = std::distance(first, last);
            m_start = allocate(n);
            std::uninitalized_copy(first , last , m_start);
            m_finish = m_end_of_storage = m_start + n;
        }

        //https://www.doubao.com/thread/w194ca12fc19e2bfa
        vector(const vector& other)
            :vector(other.begin(), other.end()){}
        vector(vector&& other) noexcept
        {
            this->m_start = other.m_start;
            this->m_finish = other.m_finish ; 
            this->m_end_of_storage = other.m_end_of_storage;
            //把other所有指针置nullptr，它析构时判定无内存可释放，自然不会删你已经接管的堆内存。
            other.m_end_of_storage = other.m_finish = other.m_start = nullptr;
        }

        //拷贝赋值
        vector& operator=(const vector& other)
        {
            if(this == &other)
            {
                return *this;
            }
            //其他节点元素 大于 预设容量
            if(other.size() > capacity())
            {
                vector tmp(other);
                tmp.swap(*this);  //出函数，tmp资源释放，即原本的this资源释放
            }
            //其他元素大于本容器元素， 但小于容量
            else if(other.size() > size())
            {
                std::copy(other.begin() , other.begin() + size() , m_start);
                iterator m_finish  = std::uninitialized_copy(other.begin() + size() , other.end() ,m_finish);
            }
            else{
                iterator new_finish = std::copy(other.begin() , other.end() ,m_start);
                //析构原本*this 中剩余位置后的元素
                std::destroy(new_finish , end());
                m_finish = new_finish;
            }
            return *this;
        }



        void swap(vector& other)
        {
            //交换两个vector的资源
            std::swap(m_start ,other.m_start);
            std::swap(m_finish,other.m_finish);
            std::swap(m_end_of_storage ,other.m_end_of_storage);

            // 等价手写逻辑
            // auto temp = m_start;
            // m_start = other.m_start;
            // other.m_start = temp;
        }
        
        //预留空间
        void reserve(size_type n)
        {
            if(this->capacity()>n ){
                return;
            }
            //当前内存小于预留
            iterator new_start = allocate(n);
            iterator new_finish =std::uninitialized_copy(begin() ,end() ,new_start);
            std::destroy(begin()  , end());
            deallocate(m_start);
            this->m_start = new_start;
            this->m_finish = new_finish;
            m_end_of_storage = m_start + n;

        }

        ~vector()
        {
            //destroy遍历 [first, last) 范围内的每一个对象，调用它们的析构函数
            std::destroy(begin() , end());  //析构资源
            deallocate(m_start);            //释放内存
        }

    };
}
    
