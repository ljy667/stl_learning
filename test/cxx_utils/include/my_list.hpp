
#pragma once 
#include "List_Allocator.hpp"
#include <iterator>
#include <iostream>
#include <memory>
#include <cstddef>
/* list:
    iterator
    *
    ->
    ==
    !=
    ++
    -- */

//list 基本元素为 NODE,不是T（valuetype） 本身

template<typename K, typename V>
struct ListNode
{
    K key;
    V value;
    ListNode* next;
    ListNode* prev;
    /* data */

    template<typename ...Args>
    ListNode (const K* k, Args&& ...args) : key(k) , value(std::forward<Args>(args)...), prev(nullptr) , next(nullptr){}
};

template<class T>
struct  List_Node
{
    List_Node<T>* prev ;
    List_Node<T>* next ;
    T data;
};

template <class T>
class List_Iterator{
    using value_type = T;
    using pointer = value_type*;
    using reference = value_type&;
    using iterator_category = std::bidirectional_iterator_tag;

    using Node = List_Node<T>;
    using iterator = List_Iterator<T>;

    public:
    Node* m_pNode ;
    List_Iterator() :m_pNode(nullptr) {};
    List_Iterator(Node* const m_pNode) :m_pNode(m_pNode){};
    //拷贝构造
    List_Iterator(const iterator& other):m_pNode(other.m_pNode){};

    //析构不用写，不涉及资源的管理

    reference operator*() const {return m_pNode->data; }

    /* m_pNode：指向节点结构体 / 类的指针成员变量
    m_pNode->data：通过指针访问该节点的 data 数据成员
    &：取地址运算符，整体为获取节点 data 成员的内存地址 */

    //pointer operator-> () const { return &m_pNode->data; } //指向储存Node
    
    Node* operator-> () const {return m_pNode;}

    // 迭代器的比较 ，即判断两个迭代器是否指向同一个 Node
    bool operator == (const iterator& other) const { return m_pNode == other.m_pNode;}
    bool operator != (const iterator& other) const { return !(*this == other); }
    
    iterator operator++ (int)
    {
        Node* tmp = m_pNode;
        m_pNode = m_pNode ->next ; //更换迭代器的m_pNode对象
        return tmp;
    }
    
    iterator& operator--()
    {
        m_pNode = m_pNode->prev;
        return *this;
    }

    iterator operator--(int) 
    {
        Node* tmp = m_pNode;
        m_pNode = m_pNode->prev;
        return tmp;
    }

};  



//list 可能有非常复杂的内存分配机制
template<class T , class Alloc = MyAllocator<List_Node<T>>>
class MyList
{
    public:
        using value_type = T;
        using pointer = value_type*;
        using reference = value_type& ;
        using size_type = std::size_t ;
        using Node = List_Node<T>;
        using iterator = List_Iterator<T> ;

    private:
        Node* m_pDummy ;

        static Node* allocNode(){
            return Alloc::allocate(1) ;
        }
        static void deallocate(Node* p)
        {
            Alloc::deallocate(p);
        } 

        static Node* createNode(const value_type& value){
            Node* newNode = allocNode();
            //这里需求 value_type， 具有拷贝构造函数，
            new (&newNode -> data) value_type(value) ;
            
            return newNode ;

        }
    public:
        MyList(){

            /* 尾节点 next → 哨兵
            哨兵 next → 头节点
            头节点 prev → 哨兵
            哨兵 prev → 尾节点 */

            m_pDummy = allocNode();
            m_pDummy -> prev = m_pDummy;
            m_pDummy -> next = m_pDummy;

            //初始m_pDummy自己连着自己
        }
        
        iterator insert(iterator pos , const value_type& value){
           Node* node_ptr = createNode(value) ; 
           node_ptr -> next = pos.m_pNode ;
           node_ptr -> prev = pos.m_pNode ->prev ;
           pos.m_pNode -> prev -> next = node_ptr ;
           pos.m_pNode -> prev = node_ptr ; 

           return node_ptr ;
        }

        //批量插入
        iterator insert(iterator pos , size_type count , const value_type& value){
            iterator it = pos ; 
            for (size_type i = 0 ; i < count ; i++ )
            {
                it = insert(it , value) ;
            }
            return it ;
        }

        // 函数类型：模板函数，InputIt 是输入迭代器模板参数，适配任意输入迭代器类型
        // 功能：在容器的指定位置，插入另一个迭代器区间 [first, last) 内的所有元素
        template<class InputIt>
        iterator insert(iterator pos , InputIt first , InputIt last)
        {
            return nullptr;
        }

        iterator erase (iterator pos)
        {
            Node* prevNode  = pos.m_pNode->prev;
            Node* nextNode  = pos.m_pNode->next;
            prevNode->next = nextNode; //后继
            nextNode->prev = prevNode;  //前驱
            
            std::destroy_at(&(pos->data));
            this->deallocate(pos.m_pNode);
            
            //返回下一个迭代器

            return iterator(nextNode);
            //delete 是先析构再释放， operator是先析构再释放
        }

        iterator erase(iterator first , iterator last)
        {   //从first迭代器开始，往后删除，直到last(不删除last)
            iterator it = first ; 
            while( it != last){
                it = erase(it);
            }
            return it ;
        }
        

        void show() const {
            for (auto it = begin() ; it != end(); it++){
                std::cout << *it << '\n' ;
            }
        }

        //清空容器所有元素
        void clear(){
            Node* cur = this -> m_pDummy->next;  //从第一个节点开始
            while(cur != m_pDummy)
            {
                Node* next = cur-> next;
                std::destroy_at(&cur -> data);  //析构元素
                
                deallocate(cur);  //释放内存
                cur = next;
            }

            //重置哨兵节点关系
            m_pDummy ->next = m_pDummy;
            m_pDummy ->prev = m_pDummy;

            //消除链路只能手动修改节点prev/next

            //无需cur->next/prev =  nullptr ,因为cur指向的节点内存已经还给系统
            //并且这里的cur只是程序中的临时指针，出函数自动销毁，因此也无需cur = nullptr

        }

        ~MyList(){
            clear();
            //清空哨兵节点,释放内存
            deallocate(m_pDummy);
        }


        // Node* begin() const {return this -> m_pDummy->next ;}
        // Node* end() const {return this -> m_pDummy ;} 
        // Node* begin() {return this -> m_pDummy->next ;}
        // Node* end() {return this -> m_pDummy;} 

        iterator begin() {return iterator(m_pDummy->next) ;}
        iterator end() {return iterator(m_pDummy) ; }

        iterator begin() const {return iterator(m_pDummy->next) ;}
        iterator end() const {return iterator(m_pDummy) ; }
};


