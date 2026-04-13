
#pragma once 
#include "List_Allocator.hpp"
#include <iterator>

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

    Node* m_pNode ;
    public:
    List_Iterator() :m_pNode(nullptr) {};
    List_Iterator(Node* const m_pNode) :m_pNode(m_pNode){};
    //拷贝构造
    List_Iterator(const iterator& other):m_pNode(other.m_pNode){};

    //析构不用写，不涉及资源的管理

    reference operator*() const {return m_pNode->data };

    /* m_pNode：指向节点结构体 / 类的指针成员变量
    m_pNode->data：通过指针访问该节点的 data 数据成员
    &：取地址运算符，整体为获取节点 data 成员的内存地址 */

    pointer operator-> () const { return &m_pNode->data }; //指向储存Node
    
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

    iterator operator-- (int) 
    {
        Node* tmp = m_pNode;
        m_pNode = m_pNode->prev;
        return tmp
    }

};  



//list 可能有非常复杂的内存分配机制
template<class T , class Alloc = MyAllocator<T>>
class MyList
{

};


