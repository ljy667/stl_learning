
#include "my_list.hpp"
#include "vector.hpp"
#include <vector>
#include <numeric>
#include <functional>
#include <algorithm>
#include <iostream>
#include <complex>

void TestList(){
    MyList<int> myList;

    for (int i = 0 ; i < 5; i++ ){
        myList.insert(myList.end() ,i);
    }

    myList.show();

    auto it = myList.end();
    --it ;
    myList.erase(it);

    myList.show();


    myList.erase(myList.begin() , --myList.end());
    myList.show() ;

    myList.assign(10, 60);
    myList.show() ;
    
}

void TestVector(){
    cxx::vector<std::string>  myVector  {} ;
    myVector.show() ;

    cxx::vector<std::string> vec2(5 , "0");
    vec2.show();

    cxx::vector<std::string> vec3(std::move(vec2));
    vec2.show();
    vec3.show();

    myVector.swap(vec3);
    vec3.show();
    myVector.show();

    cxx::vector<std::string> vec5(7 , "9");
    cxx::vector<std::string> vec;

    vec = vec5;
    vec.show();

    vec = std::move(vec5);
    vec.show();
    vec5.show();

}


int ___main(){
    // TestList();
    // TestVector();
    // return  0 ;

    std::vector<int> v(10,2);
    //std::partial_sum 两个元素逐个元素累加覆盖，逐个更新第二个元素
    std::partial_sum(v.cbegin() , v.cend() , v.begin());
    std::cout << "在这些数字中" ;

    //std::ostream_iterator：输出流迭代器，把复制的数据直接转发给std::cout
    //简洁高效，无需手动写循环遍历容器，一行代码完成容器打印。
    std::copy(v.cbegin() , v.cend()  , std::ostream_iterator<int>(std::cout , " "));

    if(std::all_of(v.cbegin() , v.cend() , [](int i){return i%2 == 0;}))
        std::cout <<"所有数字都是偶数 " ;
    
    using namespace std::placeholders;

    if(std::none_of(v.cbegin(), v.cend() , std::bind(std::modulus<>() , _1 ,2) ) )
        std::cout << "没有任何数字是奇数\n";

    struct DivisibleBy
    {
        const int d;
        DivisibleBy(int n) : d(n) {}
        bool operator() (int n) const {return n%d == 0 ;}
        //重载了函数调用操作符 operator()，使得该结构体的实例可以像函数一样被调用
    };

    if(std::any_of(v.cbegin() , v.cend() ,DivisibleBy(7)))
        std::cout << "至少一个数字可以被7整除 \n";
}

bool is_even(int i)
{
    return i % 2 == 0; 
}

void example_contains()
{
    //auto 自动推导为 std::initializer_list<int>
    const auto haystack = {1,2,3,4} ;
    for(const int needle: {3,5})
        if(std::find(haystack.begin() , haystack.end() , needle) == haystack.end())
            std::cout<< "haystack 不包含" << needle << '\n' ;
        else
            std::cout<< "haystack 包含" << needle << '\n' ;
}

void example_predicate()
{
    for(const auto& haystack :{std::array {3,1,4} , {1,3,5}})
    {
        const auto it = std::find_if(haystack.begin() , haystack.end() , is_even);
        if(it != haystack.end())
        {
            std::cout << "haystack 包含偶数：" << *it << '\n';
        }
        else
        {
            std::cout << "haystack 不包含偶数\n";
        }
    }
}

auto print_result = [](auto result , const auto& v )
{
    result == v.end()
        ? std::cout << "未找到序列\n"
        : std::cout << " 最后一次在位置 " << std::distance(v.begin() , result)
};

int main(){
    const auto v = {1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4} ;
    for(const auto& x : {std::array{1,2,3} , {4,5,6}})
    {
        auto  iter = std::find_end(v.begin() , v.end() , x.begin() , x.end());
        print_result(iter ,v);
    }

    for(const auto& x: {std::array{-1,-2,-3} , {-4,-5,-6}})
    {
        auto iter = std::find_end(v.begin() , v.end() , x.begin() , x.end(),
                [](int x , int y){
                    return std::abs(x) == std::abs(y);
                }) ;
        print_result(iter , v);
    }
}

//匿名 lambda 打印序列，标记指定下标元素，格式：id {元素 1,[标记元素], 元素 3 }
auto print_sequence = [](const auto id , const auto& seq , int pos = -1)
{
    std::cout << id <<  "{";
    for(int i{} ; const auto& e :seq)
    {
        const bool mark{ i == pos };
        std::cout << (i++ ? "," : "");
        std::cout << (mark ?  "[" : "" )<< e << (mark ? "]" : "") ;
    }
    std::cout << " }\n";
};

int main()
{
    const std::vector<int> v{0,2,3,25,5};
    const auto t1 = {19, 10 ,3 , 4};
    const auto t2 = {1, 6, 7 ,9} ;

    auto find_any_of  = [](const auto& v , const auto& t)
    {
        const auto result = std::find_first_of(v.begin() ,v.end() , 
                                                t.begin() , t.end());
        if(result == v.end())
        {
            std::cout <<  "v 和 t 中没有相等的元素\n"; 
            print_sequence("t=" ,t);
            print_sequence("v=" ,v);
        }
        else
        {
            const auto pos = std::distance(v.begin() , result) ; 
            std::cout << "在位置" << pos << "找到匹配（" << *result << ")\n";
            print_sequence("t = " , t) ;
            print_sequence("v = " , v , pos);
        }
    };

    find_any_of(v, t1);
    find_any_of(v, t2);
};

//std::adjacent_find
//C++ 标准库算法，
//查找容器 / 数组中第一对相邻且相等的元素，返回指向这对元素第一个位置的迭代器
int main(){
    std::vector<int> v1{0,1,2,3,40,40, 41, 41, 5};
    auto i1 = std::adjacent_find(v1.begin() , v1.end());
    if(i1 == v1.end())
    {
        std::cout << "没有相邻的匹配元素\n";
    }
    else
        std::cout << "第一对相邻的元素位于"
                  << std::distance(v1.begin() , i1) << ", *i1 = "
                  << *i1 << '\n' ;
    //查找规则：std::greater<int>()，前元素 > 后元素
    auto i2 = std::adjacent_find(v1.begin() , v1.end() ,std::greater<int>());
    if(i2 == v1.end())
        std::cout << "整个vector已经是升序的\n" ; 
    else
        std::cout << "非降序子序列中最后的元素位于 "
            <<std::distance(v1.begin() , i2) << ", *i2= "  << *i2 << '\n' ;
}

//std::count ,std::count_if
int main(){
    std::array v = {1,2,3,4,5,6,7,8,9,10};
    std::cout << "v:" ;
    std::copy(v.cbegin() , v.cend() , std::ostream_iterator<int>(std::cout, " "));
    std::cout << '\n' ;

    for(const int target : {3 , 4, 5})
    {
        const int num_items = std::count(v.cbegin() ,v.cend() , target) ; 
        std::cout << "目标数字：" << target << "，计数：" << num_items << '\n';
    }

    int count_div4 = std::count_if(v.begin() , v.end() , [](int i){return  i%4 == 0 ; });
    std::cout << "能被4整除的数字个数 : " << count_div4 << '\n' ;

    auto distance = [](auto first , auto last){
        return std::count_if(first , last , [](auto){return true;});
    };
    static_assert(distance(v.begin() ,v.end()) == 10);

    //元素类型：复数 std::complex<double>，实部虚部均为 double 浮点
    std::array<std::complex<double> ,3> nums{{{4, 2}, {1, 3}, {4, 2}}} ; 
    auto c = std::count(nums.cbegin(), nums.cend(), {4, 2});
    auto c = std::count(nums.cbegin(), nums.cend(), std::complex<double>{4, 2});

    assert( c == 2 );
}

    template<class InputIt1 , class InputIt2 , class BinaryPred>
    std::pair<InputIt1 , InputIt2>
        mismatch(InputIt1 first1, InputIt1 last1 , InputIt2 first2, BinaryPred p)
        {
            while (first1 != last1 && p(*first1 , *first2))
            {
                ++first1 , ++first2;
            }
            return std::make_pair(first1, first2);
        }

    template<class InputIt1, class InputIt2>
    std::pair<InputIt1, InputIt2>
            mismatch(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2)
        {
            while (first1 != last1 && first2 != last2 && *first1 == *first2)
                ++first1, ++first2;
            
            return std::make_pair(first1, first2);
        }

        //此程序确定同时在给定字符串起始与在其结尾按逆序同时找到的最长子串（可能重叠）
    std::string mirror_ends(const std::string& in)
    {
        //std::string(ite1, ite2) ,应该返回同一个字符串容器的迭代器1到2之间的元素
        return std::string(in.begin() , 
                    std::mismatch(in.begin() ,  in.end() , in.rbegin()).first);
    }



    // std::equal
    template<class ForwardIt1, class ForwardIt2>
    constexpr
    ForwardIt1 search(ForwardIt1 first , ForwardIt1 last,
                    ForwardIt2 s_first , ForwardIt2 s_last)
        {
            while(true)
            {
                ForwardIt1 it = first ; 
                for(ForwardIt2 s_it = s_first;   ;++it  ,++s_it)
                {
                    if(s_it == s_last)
                        return first;  //retrun退出while循环
                    if(it == last)
                        return last;  
                    if(!(*it == *s_it))
                        break;
                }
                ++first;
            }
        }

    /*  语法作用：模板形参别名定义，从正向迭代器ForwardIt提取迭代器关联的值类型，赋值给类型别名T
        核心组件：std::iterator_traits迭代器萃取工具，统一获取迭代器内嵌类型
        约束：仅适配正向迭代器ForwardIt，T等价于迭代器指向元素的类型
        用途：泛型算法中无需手动传入元素类型，自动推导迭代器存储值类型 */
    
    //搜索连续为value的元素
    template<class ForwardIt , class size,
    class T = typename std::iterator_traits<ForwardIt>::value_type>
    ForwardIt search_n(ForwardIt first , ForwardIt last , 
                        std::size_t count ,const T& value)
    {
        if(count <= 0)
            return first;

        //逻辑，找到第一个==value的元素
        for( ; first != last ;  ++first)
        {
            if(!(*first == value))
                continue;
            ForwardIt candidate = first;
            for(std::size_t cur_count = 1 ; true ; ++cur_count)
            {
                if(cur_count >= count)
                    return candidate; //成功

                ++first ;
                if(first == last)
                    return last;
                
                //没找到连续的
                if(!(*first == value))
                    break;
            }
        }
        return last;
    }

/* (void)是强制类型转换，把这个返回值强行转为void（无类型、无返回），
    告诉编译器：我明确知道有返回值，故意不用，不用报警。 */

template<class InputIt , class OutputIt >
OutputIt copy(InputIt first , InputIt last , OutputIt d_first)
{
    for( ; first != last  ; (void) ++first , (void) ++d_first)
        *d_first = *first ; 
        return d_first  ;
}

template<class InputIt , class OutputIt , class UnaryPred>
OutputIt copy_if(InputIt first , InputIt last , 
                OutputIt d_first , UnaryPred pred)
                {
                    for(  ; first != last ; ++first)
                        if(pred(*first))
                        {
                            *d_first = *first;
                            ++d_first  //first 和 d_first同时进行
                        }
                }



int main()
{
    std::vector<int> from_vector(10);
    //功能：遍历from_vector全部元素，从下标 0 开始，
    //依次给容器赋值连续整数：首元素 = 0、次元素 = 1，逐项 + 1。
    std::iota(from_vector.begin() ,from_vector.end() , 0);
    std::vector<int> to_vector;
    //尾插入迭代器,对迭代器执行=赋值时，自动调用容器.push_back()，在容器尾部追加元素。
    std::copy(from_vector.begin() , from_vector.end() ,std::back_inserter(to_vector));

    std::vector<int> to_vector_two(from_vector.size()) ; 
    std::copy(from_vector.begin() ,from_vector.end() ,to_vector_two.begin());
    std::cout << "to_vector中的奇数有:"
    std::copy_if(to_vecotr.begin() ,to_vector.end() , 
        std::ostream_iterator<int>(std::cout , " "),
        [](int x){return x%2 != 0 ;})

}


//std::copy_n
template<class InputIt , class Size , class OutputIt>
constexpr // c++20
OutputIt copy_n(InputIt first , Size count , OutputIt result)
{
    if(count > 0)
    {
        *result = *first ; 
        ++ result
        for(Size i = 1 ; i != count ; ++i , (void) ++result)
            *result =  *++first;
    }
    return result ; 
}

int main()
{
    std::string in {"123456789"} ;
    std::string out ; 

    std::copy_n(in.begin() ,4 ,std::back_inserter(out));
    std::cout << out << '\n';

    std::vector<int> v_in(128);
    // <numeric>
    //给容器v_in连续填充递增数值。
    //参数含义：从v_in起始迭代器到末尾迭代器，初始值为 1
    std::iota(v_in.begin() , v_in.end() , 1);
    std::vector<int> v_out(v_in.size());

    std::copy_n(v_in.cbegin() , 100 ,v_out.begin());
    std::cout << std::accumulate(v_out.begin() , v_out.end() , 0) << '\n' ;
}
//1234;5050

template<class BidirIt1 , class BidirIt2>
BidirIt2 copy_backward(BidirIt1 first , BidirIt2 last , BidirIt2 d_last)
{
    while(first != last )
        *(--d_last) = *(--last) ;
    return d_last ; 
}

#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

int main()
{
    std::vector<int> source(4);
    std::iota(source.begin(), source.end(), 1); // 填充 1, 2, 3, 4
    std::vector<int> destination(6);
    std::copy_backward(source.begin(), source.end(), destination.end());
    std::cout<< "destination 包含：" ;
    for(auto i:destination)
        std::cout << i << ' ';
    std::cout << '\n' ;
}

//destination 包含：0 0 1 2 3 4
//std::move(v.begin(), v.end(), std::back_inserter(l));
template<class InputIt  , class OutputIt>
OutputIt move(InputIt first , InputIt last , OutputIt d_first)
{
    for( ; first != last ; ++d_first , ++first)
        *d_first - std::move(*first)

    return d_first;
}

template<class BidirIt1 , class BidirIt2>
BidirIt2 move_backward(BidirIt1 first , BidirIt1 last , BidirIt2 d_last)
{
    while(first != last)
    {
        *(--d_last)  = std::move(*(--last));
    }
    return d_last ;
}

namespace Ns
{
    class A 
    {
        int id {} ; 
        friend void swap(A& lhs , A& rhs)
        {
            std::cout << "swap("  <<  lhs << "," << rhs << ")\n";
            std::swap(lhs.id , rhs.id) ;
        }
        friend std::ostream& operator<< (std::ostream& os , A const& a)
        {
            return os<< "A::id = "<< a.id;
        }
    }
}

template<class ForwardIt1 , class ForwardIt2>
constexpr
void iter_swap(ForwardIt1 a , ForwardIt2 b)
{
    using std::swap;
    swap(*a , *b);
}

template<class ForwardIt>
void selection_sort(ForwardIt begin , ForwardIt end)
{
    For (ForwardIt it = begin ; it != end; it++)
        std::iter_swap(it , std::min_element(it ,end));
} 



//非类型模板参数
template<int min , int max>
int rand_int()
{
    // 静态随机分布：限定整数范围 [min, max]
    static std::uniform_int_distribution<int> dist(min, max);
    // 静态随机数引擎mt19937，只用random_device播种一次
    static std::mt19937 gen(std::random_device{}());
    // 使用引擎生成该区间内均匀随机整数
    return dist(gen);
}

template<class ForwardIt1 , class ForwardIt2>
constexpr
ForwardIt2 swap_ranges(ForwardIt1 first1 , ForwardIt1 last1 ,ForwardIt2 first2 )
{
    for( ; first1 != last1 ; ++first1 , ++first2)
        std::iter_swap(first1 ,first2);
    return first2 ;
}


auto print = [](auto comment , auto const& seq)
{
    std::cout << comment ; 
    for(const auto& e:seq)
        std::cout << e << ' ';
    std::cout << '\n' ;
};

int main ()
{
    std::vector<char> v {'a' , 'b' , 'c'};
    std::list<char> l {'1' , '2' , '3'};
    std::swap_ranges(v.begin() , v.begin()+3 ,l.begin());


}


#include <algorithm>
template<class InputIt , class OutputIt , class UnaryOp>
constexpr
OutputIt transform(InputIt first1 ,InputIt last1 ,
                 OutputIt d_first , UnaryOp unary_op)
                {
                    for( ; first1 != last1 ; ++d_first , ++ first1)
                        *d_first = unary_op(*first1);
                    return d_first ;
                }

template<class InputIt1 , class InputIt2 , class OutputIt , class BinaryOp>
constexpr
OutputIt transform (InputIt1 first , InputIt1 last , InputIt2 first2 ,
                     OutputIt d_first, BinaryOp binary_op)
                    {
                        for(; first1 != last ; d_first, ++first1, ++first2)
                            *d_first = binary_op(*first1 , *first2);
                        return d_first ; 

                    }

void binary_transform_example(std::vector<unsigned> ordinals)
{
    //对每个元素执行自身 + 自身（std::plus求和）
    std::transform(ordinals.cbegin() , ordinals.cend() ,ordinals.cbegin() ,
                    ordinals.begin() , std::plus<>{}) ;
}

int main()
{
    std::string hello("hello") ; 
    std::vector<unsigned> ordinals;
    std::copy(hello.cbegin() , hello.cend(), std::back_inserter(ordinals));
    binary_transform_example(std::move(ordinals)) ;
}