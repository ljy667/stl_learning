//快速查找  哈希o(1)  
//key-value 结构
#include <utility>
#include <tuple>
#include <cstddef>
#include <list>
#include <unordered_map>
#include <type_traits>
#include <new>
#include <stdexcept>

template<typename K,typename V ,typename K_Hash=std::hash<K> ,typename K_Eq = std::equal_to<K>>
struct LRUCache{
private:
  //保存key和映射表

    //操作list时存在遍历，并且两个结构都存储了一份key
    //可通过key直接找到value
     struct Node{
        K key;
        V value;
        Node *next;
        Node *prev;

        template<typename... Args>
        Node(const K& k, Args&&... args) :key(k), value(std::forward<Args>(args)...), prev(nullptr), next(nullptr){}
    };
public:
    LRUCache(size_t capatipy);
    ~LRUCache();
    //访问（）、插入
    //void Insert(const K& key,const V& value);

    //构造到容器里面去
    //1. key需可复制，value构造
    template<typename ...Args>
    void Insert(const K& key,Args && ...args);

    //2.key构造，value 构造，利用元组
    struct Emplace_t{};
    template<typename Tup_key,typename Tup_value>
    void Insert(Emplace_t et, Tup_key &&tup_key,Tup_value &&tup_value);

    //元组存的是构造参数，还是元组本身是构造参数，因此需防歧义参数

    V& Get(const K& key);
    template<typename... Args>
    V& Get(Args && ...args);

    const V& Get(const K& key) const;
    template<typename... Args>
    const V& Get(Args && ...args) const;
    
   

    // 用户操作 访问节点，节点需要往前挪，获取节点 操作节点
    struct Iterator{
        friend LRUCache;
        public:
            Iterator();
            Iterator(const Iterator& other); // copy constructor
            Iterator& operator=(const Iterator& other);

            bool operator==(const Iterator& other) const; 
            bool operator!=(const Iterator& other) const;
            V& operator *() ;  //操作优先级
            V* operator ->() ;  // 操作优先级
            Iterator& operator++();
            bool isNull() ;
        private:
            //迭代器内部需要保存当前节点的指针
            Node* Node_;    //调用了
            LRUCache* lru_;
    };
    //不应该提供const迭代器
    
    bool Find(const K& key) const; //加const
    Iterator Find(const K& key) ;  //不能加const

private:
    size_t capacity_;
    friend struct Iterator; 

   
    Node *head_;
    Node *tail_;
    std::unordered_map<K , Node * ,K_Hash, K_Eq> data_;
    //std::unordered_multimap<size_t ,Node *> date_;
 
    //两个key传进来 hash值一样，需判断是哪个key 
private:
    void Detach(Node* node);
    void AttachFront(Node* node);
    void EvictIfNeeded();
};


template<typename K , typename V, typename K_Hash, typename K_Eq>
LRUCache<K,V,K_Hash,K_Eq>::Iterator::Iterator() : Node_(nullptr),lru_(nullptr){}

template<typename K , typename V, typename K_Hash, typename K_Eq>
 LRUCache<K,V,K_Hash,K_Eq>::Iterator::Iterator(const Iterator& iter) : Node_(iter.Node_),lru_(iter.lru_){}

//拷贝赋值
template<typename K , typename V, typename K_Hash, typename K_Eq>
 LRUCache<K,V,K_Hash,K_Eq>::Iterator&  LRUCache<K,V,K_Hash,K_Eq>::Iterator::operator=(const Iterator& iter)
{
    if(this!= &iter)
    {
        this->Node_=iter.Node_;
        this->lru_= iter.lru_;
    }
    return *this;
}

template<typename K , typename V, typename K_Hash, typename K_Eq>
bool LRUCache<K,V,K_Hash,K_Eq>::Iterator::operator==(const Iterator &iter) const{
    return Node_==iter.Node_ && lru_ == iter.lru_;
}

template<typename K , typename V, typename K_Hash, typename K_Eq>
bool LRUCache<K,V,K_Hash,K_Eq>::Iterator::operator!=(const Iterator &iter) const{
    return !(*this == iter);
}

template<typename K , typename V, typename K_Hash, typename K_Eq>
V& LRUCache<K,V,K_Hash,K_Eq>::Iterator::operator*(){
    return this->Node_->value;
}

template<typename K , typename V, typename K_Hash, typename K_Eq>
V* LRUCache<K,V,K_Hash,K_Eq>::Iterator::operator->(){
    return &(Node_->value);
}


template<typename K , typename V, typename K_Hash, typename K_Eq>
bool LRUCache<K,V,K_Hash,K_Eq>::Iterator::isNull(){
    return Node_ == nullptr;
}

template<typename K , typename V, typename K_Hash, typename K_Eq>
LRUCache<K,V,K_Hash,K_Eq>::LRUCache(size_t capacity):capacity_(capacity),head_(nullptr),tail_(nullptr){}

template<typename K , typename V, typename K_Hash, typename K_Eq>
LRUCache<K,V,K_Hash,K_Eq>::~LRUCache(){  //处理second指向的对象，释放堆存被析构对象，再置空指针
    Node* cur = head_;
    while(cur){
        Node* next = cur->next;
        delete cur;
            // 无论put/get等方法，创建Node时必然用new（堆内存）
            // Node* node = new Node(std::forward<Args>(args)...); 
        cur = next;
    }
    head_ = nullptr;
    tail_ = nullptr;
    data_.clear();   //清除所有对象
}
    
//抽离
template<typename K , typename V, typename K_Hash, typename K_Eq>
void LRUCache<K,V,K_Hash,K_Eq>::Detach(Node* node){
    if(head_!=tail_)
    {
        if(!node){
            return;
        }
        if(node->prev){
            node->prev->next = node->next;
        }
        if(node->next){
            node->next->prev = node->prev;
        }
        if(node == head_){
            head_ = node->next;
            head_->prev=nullptr;
        }
        if(node == tail_){
            tail_ = node->prev;
            tail_->next=nullptr;
        }
        node->prev = nullptr;
        node->next = nullptr;
    }
    else 
    {return;} 
}

template<typename K , typename V, typename K_Hash, typename K_Eq>
void LRUCache<K,V,K_Hash,K_Eq>::AttachFront(Node* node){
    if(!node){
        return;
    }
    node->prev = nullptr;
    node->next = head_;
    if(head_){
        head_->prev = node;
    }
    head_ = node;
    if(!tail_){
        tail_ = node;
    }
}

template<typename K , typename V, typename K_Hash, typename K_Eq>
void LRUCache<K,V,K_Hash,K_Eq>::EvictIfNeeded(){
    while (data_.size() > capacity_ && tail_){
        Node* to_remove = tail_;
        Detach(to_remove);
        data_.erase(to_remove->key);
        delete to_remove;
    }
}

//当对LRU作添加操作时，如果找到，要将其放入首位，如果没有找到
template<typename K , typename V,typename K_Hash, typename K_Eq>
template<typename... Args>
void LRUCache<K,V,K_Hash,K_Eq>::Insert(const K& key, Args && ...args){
  //哈希表
  auto it = data_.find(key);
  
  //找到了key
  if(it != data_.end()){
      Node *node  = it->second;  //指针赋值
        //编译期判断V类型是否支持赋值操作。
        if constexpr (std::is_assignable_v<V&, V>) {
            node->value = V(std::forward<Args>(args)...);
        }
        // 不支持赋值（如无赋值运算符的类）：先手动析构旧值，
        // 再通过placement new在原内存地址原地构造新值（避免内存重新分配）
        else 
        {
            node->value.~V();
            new (&node->value) V(std::forward<Args>(args)...);
        }
        if(node != head_){
            Detach(node);
            AttachFront(node);
        }
        return;
    }
    Node* node = new Node(key, std::forward<Args>(args)...);
    data_[key] = node;
    AttachFront(node);
    EvictIfNeeded();
}

template<typename K , typename V, typename K_Hash, typename K_Eq>
V& LRUCache<K,V,K_Hash,K_Eq>::Get(const K& key){
    auto it = data_.find(key);
    if(it == data_.end()){
        throw std::out_of_range("LRUCache: key not found");
    }
    Node* node = it->second;
    if(node != head_){
        Detach(node);
        AttachFront(node);
    }
    return node->value;
}


template<typename K , typename V, typename K_Hash, typename K_Eq>
template<typename... Args>
V& LRUCache<K,V,K_Hash,K_Eq>::Get(Args && ...args){
    K key(std::forward<Args>(args)...);
    return Get(key);
}

template<typename K , typename V, typename K_Hash, typename K_Eq>
const V& LRUCache<K,V,K_Hash,K_Eq>::Get(const K& key) const{
    auto it = data_.find(key);
    if(it == data_.end()){
        throw std::out_of_range("LRUCache: key not found");
    }
    return it->second->value;
}

template<typename K , typename V, typename K_Hash, typename K_Eq>
template<typename... Args>
const V& LRUCache<K,V,K_Hash,K_Eq>::Get(Args && ...args) const{
    K key(std::forward<Args>(args)...);
    return Get(key);
}

template<typename K , typename V, typename K_Hash, typename K_Eq>
bool LRUCache<K,V,K_Hash,K_Eq>::Find(const K& key) const{
    return data_.find(key) != data_.end();
}

template<typename K , typename V, typename K_Hash, typename K_Eq>
typename LRUCache<K,V,K_Hash,K_Eq>::Iterator LRUCache<K,V,K_Hash,K_Eq>::Find(const K& key){
    Iterator it;
    auto iter = data_.find(key);
    if(iter != data_.end()){
        it.Node_ = iter->second;
        it.lru_ = this;
    }
    return it;
}

template<typename K , typename V, typename K_Hash, typename K_Eq>
typename LRUCache<K,V,K_Hash,K_Eq>::Iterator& LRUCache<K,V,K_Hash,K_Eq>::Iterator::operator++(){
    if(Node_){
        Node_ = Node_->next;
    }
    return *this;
}



int main (int argc,char *argv[])
{
    std::tuple<int> a(1);
    std::tuple<int,int> b(1,2);
}

// const 位置	                                     
// const K& key（参数）	                &：传引用避免拷贝；2. const：函数内部不会修改传入的 key（只读）	
// const V& Get(...) const（返回值）	    返回的 value 是只读的，调用者不能修改它	
// Get(...) const（函数末尾）	            这是const 成员函数：函数内部不能修改 LRUCache 的成员变量（除非成员是mutable）；
                                                        


