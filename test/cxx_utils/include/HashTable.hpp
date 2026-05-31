
#include <cstddef>

// 哈希表，同一哈希值，插入同一 链表，链表需要遍历

//扩容 ， 容器


template<class Val , class Key , class HashFn , class ExtractKey>
class HashTable;

template<class Val , class Key , class HashFn , class ExtractKey>
class HashTableIterator{
    public:
        using size_type = std::size_t;
        using difference_type = std::prtdiff_t ; 
        using value_type = Val* ;
        using pointer = value_type* ;
        using reference = value_type& ;
        using iterator_category = std::forward_iterator_tag ;   //前向迭代器
        using iterator = HashTableIterator ; 
        
    private: 
        using Node = HashTableNode<Val> ; 
        using HT = HashTable<Val , Key , HashFn ,  ExtractKey>

        Node* m_cur ; 
        const HT* m_ht ;  //用于定位切换链表时的元素索引

        HashTableIterator(Node* node , const HT* ht)
            :m_cur(node) , m_ht(ht){

        }
        
        reference operator* () const
        {
            return m_cur -> val ;
        }


        pointer operator-> () const {
            return &operator*();
        }

        bool operator == (const iteratro& other) const {
            return m_cur== other.m_cur ;  
        }
        bool operator != (const iterator& other) const {
            return !(*this == other) ;
        }
        iterator& operator++ () {
            //同一链表中的元素
            if(m_cur -> next != nullptr){
                m_cur = m_cur ->next ;
                return *this ;
            }else{
                for(size_type idx = m_ht->getBucketIdx(m_cur->value) + 1 ; idx < m_ht->m_buckets.size() ; ++idx ){
                    //找到第一个不是最后一个桶的的桶
                    if(m_ht -> m_buckets[idx] != nullptr) 
                    {
                        m_cur = m_ht -> m_buckets[idx];
                        return *this
                    }
                }
            }
            m_cur = nullptr ;
            return *this ;
            //不是同一链表中的元素
        }
};


template <class Val , class Key , class HashFn ,class ExtractKey >
class HashTable
{
public:
    using value_type = Val ; 
    using Key_type = key;
    using size_type = std::size_t ; 
    using differece_type = std::ptrdiff_t ;
    using iterator = HashTableIterator<val, key  , HashFn , ExtractKey>;
    using Node = HashTableNode<Val> ; 

    friend iterator ; 

private:
    HashFn m_hash ;  //哈希函数
    ExtractKey m_get_key ; 
    std::vector<Node*> m_buckets;   //桶，存储了一系列指针 ， 指向了一系列链表
    size_type m_size;  //元素的总数量

    //素数表实现，专门用于 vector/hash_map 等容器扩容时选取最优容量（用素数减少哈希冲突[避免挤在一个链表里]、提升内存分配效率）
    static const int __stl_num_primes = 28; 
    inline static const unsigned long __stl_prime_list[__stl_num_primes] = 
    {
        53UL,          97UL,          193UL,         389UL,         769UL,
        1543UL,        3079UL,        6151UL,        12289UL,       24593UL,
        49157UL,       98317UL,       196613UL,      393241UL,      786433UL,
        1572869UL,     3145739UL,     6291469UL,     12582917UL,    25165843UL,
        50331659UL,    100663319UL,   201326639UL,   402653189UL,   805306457UL,
        1610612741UL,  3221225473UL,  4294967291UL
    };

    //获取下一个质数
    static unsigned long __stl_next_prime(unsigned long __n)
    {
        const unsigned long* __first  =  __stl_prime_list ; 
        const unsigned long* __last = __stl_next_prime + stl_num_primes ; 
        const unsigned long* pos = lower_bound(__first , __last , __n);
        return pos = __last ?*(__last - 1) :*pos ; 
    }

    size_type nextPrime(size_type n) const ;
    {
        return __stl_next_prime(n) ;
    }

    //返回桶中的idx
    size_type  getBucketIdx(const value_type& val) const
    {
        //m_get_key(val) 获取val对应的key ,再通过hash函数计算哈希值
        // 
        return m_hash(m_get_key(val)) % m_buckets.size();
    };


    size_type getBucketIdx(const value_type& val , size_t bucketSize) const 
    {
        return m_hash(m_get_key(val)) % bucketSize;
    }

    //load_factor代表哈希表的拥挤程序
    constexpr float max_load_factor const {
        return 1.0 ;
    }

    //重排哈希
    void rehash(size_type newSizeHint){

    };

    Node* allocNode(){
        //operartor只分配内存，不构造对象
        return static_cast<Node*>(operator new(sizeof(Node))) ; 
    }

    void deallocNode(Node* p){
        operator delete(p) ;
    } ;

public:
    HashTable(size_type  bucketSizeHint = 1 , const HashFn& hash = HashFn() , const ExtractKey& getKey = ExtractKey())
        :m_hash(hash) , m_get_key(getKey) 
    {
        /* 哈希表的桶（bucket）数量必须是质数！构造函数里取 >= 建议值 的第一个质数，是为了：
            满足哈希表的底层硬性要求（质数桶数 = 最少哈希冲突）
            保证桶数量合法（≥1，不能是 0）
            适配用户传入的建议容量 */

        size_type nBuckets = nextPrime(bucketSizeHint) ;
        // 初始化对应数量的空桶数组（赋值为 nullptr）
        m_buckets.assign(nBuckets ,nullptr) ;  
    };
 
    //增删改查

    iterator insert(const value_type& val);
    size_type erase(const Key_type& key) ;
    iterator find(const Key_type& key);

    iterator begin() ;
    iterator begin() const ;
    iterator end() ;
    iterator end() const ;

};