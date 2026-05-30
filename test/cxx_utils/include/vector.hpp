
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
        //移动赋值
        vector& operator=(vector&& other) noexcept
        {
            if(this == &other)
            {
                return *this;
            } 
            //本身所持有的资源释放掉

            //触发元素析构
            std::destroy(begin() , end());
            //释放内存
            deallocate(m_start);
            m_start = other.m_start;
            m_finish = other.m_finish;
            m_end_of_storage = other.m_end_of_storage;
            other.m_start = other.m_finish = other.m_end_of_storage = nullptr;
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

        //方括号运算符
        reference operator[](size_type n)
        {
            return *(begin() + n);
        }

        const_reference operator[](size_type n) const
        {
            return *(begin() + n);
        }

        reference front(){
            return *begin();
        }

        const_reference front() const{
            return *begin();
        }

        //end为尾后迭代器
        /* end 属于尾后迭代器，不指向容器有效元素
        仅作遍历终止标记，不可解引用、自减取值
        遍历判定：元素迭代器不等于 end 即为有效位置 */
        reference back(){
            return *(end()-1);
        }

        const_reference back() const{
            return *(end()-1);
        }

        T* data(){
            return begin();
        }

        const T* data() const
        {
            return begin();
        }

        bool empty() const
        {
            return begin() == end();
        }

        void clear()
        {
            //清空元素，并非释放内存
            std::destroy(begin() , end());
            //m_finish = m_start
            m_finish = begin();
        }
    
        //重头戏insert
        iterator insert(iterator pos , const value_type& val)
        {
            //判断容器是否还有空间
            if(m_finish != m_end_of_storage)
            {   //移动 m_finish - 1 位置的元素到m_finish
                if(pos != m_finish){
                    std::construct_at(m_finish , std::move(*(m_finish - 1)));
                    std::move_backward(pos , m_finish - 1 , m_finish);
                    *pos = val ;
                    ++m_finish;
                    return pos;}
                else{
                    //直接在最后一个位置构造
                    std::construct_at(m_finish , val);
                    ++ m_finish ;
                    return pos ; 
                }
            }
            else{
                size_type new_size = std::max( 2 * size() , size() + 1);
                iteraotr new_start = allocate(new_size) ;
                iterator new_finish = std::uninitialized_move( m_start  , pos , new_start);
                //即在new_finish处 增加一个元素
                new_finish = std::construct_at(new_finish , val);
                ++new_finish;
                new_finish = std::uninitialized_move(pos , m_finish , new_finish);

                std::destroy(begin() ,end());
                deallocate(m_start) ; 
                m_start = new_start ; 
                m_finish = new_finish ;
                m_end_of_storage m_start + new_size();
            }  
        }


        iterator insert(iterator pos , value_type&& val)
        {
            //判断容器是否还有空间
            if(m_finish != m_end_of_storage)
            {   //移动 m_finish - 1 位置的元素到m_finish
                if(pos != m_finish){
                std::construct_at(m_finish , std::move(*(m_finish - 1)));
                std::move_backward(pos , m_finish - 1 , m_finish);
                *pos = std::move(val);
                ++m_finish;
                return pos;}
                else{
                    //直接在最后一个位置构造
                    std::construct_at(m_finish , std::move(val));
                    ++ m_finish ;
                    return pos ; 
                }
            }
            else{
                size_type new_size = std::max( 2 * size() , size() + 1);
                iteraotr new_start = allocate(new_size) ;
                iterator new_finish = std::uninitialized_move( m_start  , pos , new_start);
                //即在new_finish处 增加一个元素
                new_finish = std::construct_at(new_finish , std::move(val));
                ++new_finish;
                new_finish = std::uninitialized_move(pos , m_finish , new_finish);

                std::destroy(begin() ,end());
                deallocate(m_start) ; 
                m_start = new_start ; 
                m_finish = new_finish ;
                m_end_of_storage m_start + new_size();
            }
        }

        iterator insert(iterator pos , size_type n , const value_type& val)
        {
            if(n == 0)
            {
                return pos ;
            }

            //1.无需重新分配内存，剩余可分配内存大于所插入元素个数
            //一开始pos后的移动赋值，把新元素移动构造到pos前
            if(size_type(m_end_of_storage - m_finish) >= n) {
                //强制转换为无符号整型
                size_type elems_after = m_finish - pos;
                if (n < elems_after) {
                   /*  template<class InputIt, class ForwardIt>
                    ForwardIt uninitialized_move(InputIt first, InputIt last, ForwardIt d_first);*/
                    std::uninitialized_move(m_finish - n , m_finish , m_finish)
                    //std::move存在内存重叠问题（move原区域和现区域存在重叠，存在覆盖导致数据丢失问题）
                    //std::move_backward,从最后一个元素往后搬，
                    std::move_backward(pos ,m_finish-n , m_finish)

                    std::fill_n(pos,n,val);
                    m_finish += n;
                    return pos;
                }
                else{
                    //2 n>较大 ， 大于原本pos后元素数量，但整体还是小于capitcy
                    //pos前部分拷贝赋值，后面拷贝构造

                    //先移动pos位置原本元素 到 pos+n 后， 因此可预留pos 后 n个位置给新元素
                    std::uninitialized_move(pos , m_finish , pos + n) ;
                    std::fill(pos , m_finish , val) ; 
                    std::uninitialized_fill(m_finish , pos + n , val) ;
                    m_finish += n ;
                    return pos ; 
                }
            }
            else {
                //3.剩余capitcy ，开辟足够大的内存空间，然后将所有元素依次处理插入
                //size() 元素个数   ， 通过size() + n, 避免频繁扩容
                size_type new_size = std::max(2 * size() , size() + n);
                iterator new_start = allocate(new_size) ;
                iterator new_finish = std::uninitialized_move(m_start , pos , new_start) ; 
                iteraotr ret = new_finish ;
                
                iterator new_finish = std::uninitialized_fill_n(new_finish , n , val);
                new_finish  = std::uninitialized_move(pos , m_finish , new_finish);

                std::destroy(begin() , end());
                deallocate(m_start);

                m_start = new_start ;
                m_finish = new_finish ; 
                m_end_of_storage = m_start + new_size ;

                return ret ; 
            }
        }

        iterator erase(iterator pos) {
            return this-> erase (pos , pos+1) ; 
        }

        iterator erase(iterator first , iterator last)
        {
            iterator new_finish =  std::move(last , m_finish , first) ;
            std::destroy(new_finish , m_finish) ; 
            m_finish = new_finish ;
            return first ;  
        }

        //尾部加入元素
        void push_back(const value_type& val)
        {
            insert( m_finish , const value_type& val );
        }

        void push_back(value_type&& val)
        {
            insert(m_finish , value_type&& val) ;        
        }

        void pop_back()
        {
            //把结尾元素直接释放掉,指针往前移动一个
            std::destroy( end() - 1) ; 
            deallocate(end() - 1);
            --m_finish;
        }
        

        ~vector()
        {
            //destroy遍历 [first, last) 范围内的每一个对象，调用它们的析构函数
            std::destroy(begin() , end());  //析构资源
            deallocate(m_start);            //释放内存
        }
    };
}
    
