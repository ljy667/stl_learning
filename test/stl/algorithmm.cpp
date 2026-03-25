
#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include <tuple>
#include <array>
#include <cmath>
#include <numeric>

template<typename T, typename P>
std::vector<T> Filter(std::vector<T>& vec, P pred) 
{
    std::vector<T> res;
    std::copy_if(vec.begin(), vec.end(), std::back_inserter(res) , pred);
    return res;
}
// std::back_inserter
// std::back_insert_iterator


//std::set_difference
void  set_diff_test(std::vector<int> nums1,std::vector<int> nums2)
{
    std::vector<int> result;
    
    std::sort(nums1.begin(),nums1.end());
    std::sort(nums2.begin(),nums2.end());
    
    std::set_difference(nums1.begin() , nums1.end(),
                        nums2.begin(),nums2.end(),
                        std::back_insert_iterator<std::vector<int>>(result));

    for(auto v:result)
        std::cout <<v<<" ";
}


std::tuple<std::vector<int>::iterator , std::vector<int>::iterator>
part_test(std::vector<int>& numbers)
{   
    //返回迭代器，代表两组数的边界
  /*   [begin,bound1)   bound1 之前是满足条件的，bound1不满足条件
    [bound1,end)  不满足条件 */
    //进行分组
    auto bound1 = std::partition(numbers.begin(),numbers.end(),[](int v){return v%3 == 0;});
    auto bound2 = std::partition(bound1 ,numbers.end() , [](int v){return v%3 ==1; });
    std::for_each(numbers.begin(),bound1, [](int v){std::cout<<v << " ";}); //bound1为此次遍历结束的迭代器
    std::for_each(bound1, bound2, [](int v){std::cout<<v << " ";});
    std::for_each(bound2 ,numbers.end(), [](int v){std::cout<<v << " ";});
    return  std::make_tuple(bound1,bound2) ; 
}

//计算浮点数序列方差
void cal_fang ()
{
    const int N =10000;
    std::array<double,N> numbers;
    std::generate(numbers.begin(),numbers.end(),[](){return (double)rand() / (double)(RAND_MAX); });

    auto avg = std::accumulate()

    
    auto result = std::transform_reduc(numbers.begin(), numbers.end() , 0.0 , 
                                        [](auto first , auto second){return first + second ;},
                                        [avg](auto val){auto t = val - avg ; return t*t ;})/N;
}


// std::set_difference
int main (void)
{

    std::string word = "apple";
    //string::find() 未找到目标子串时，返回 npos，本质是无符号整数类型的最大值，常用作查找失败的返回标识

    auto hasword = [&word](const std::string& s){ return s.find(word) != -1;};
    std::vector<std::string> words {"an apple" , "big apple" , "orange" , "green apple"} ;
    std::vector<std::string> filter = Filter(words, hasword);
    
    //c++20
    // std::vector<std::string> res;
    // std::ranges::copy_if(words,std::back_inserter(res), hasword);

    for(auto  s:filter)
        std::cout <<s<<" "<<std::endl;
}