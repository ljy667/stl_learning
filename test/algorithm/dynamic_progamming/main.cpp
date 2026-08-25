#include <vector>
#include <algorithm>
/* 记忆化搜索是一种“从顶至底”的方法：我们从原问题（根节点）开始，
递归地将较大子问题分解为较小子问题，直至解已知的最小子问题（叶节点）。
之后，通过回溯逐层收集子问题的解，构建出原问题的解。

与之相反，动态规划是一种“从底至顶”的方法：从最小子问题的解开始，
迭代地构建更大子问题的解，直至得到原问题的解。 */

//https://www.doubao.com/thread/w7fd66cab3b4ce09f


int climbingStairDP(int n)
{
    if(n == 1 || n == 2)
    return n ; 

    std::vector<int> dp(n+1) ;
    // 初始 ， 预设最小子问题解
    dp[1] = 1; 
    dp[2] = 2;
    
    for(int i = 3 ; i<= n ; i++)
    {
        dp[i] = dp[i-1]  + dp [i-2] ;
    }
}
//无须使用一个数组 dp 来存储所有子问题的解，而只需两个变量滚动前进即可

int climbingStairsDPComp(int n)
{
    if(n == 1 || n == 2)
        return n ;
    int a = 1, b = 2;
    for(int i = 3 ; i <= n ; i++)
    {   
        int tmp = b ;
        b = a+b ; 
        a = tmp ;
    }
    return b ; 
}


//盛最多的水 , 贪心题目
int maxArea(std::vector<int>& height)
{
    int res = 0;
    //维护双指针
    for(int i = 0 ,j = height.size() -1  ; i< j ;  )
    {   
        res = std::max(res , std::min(height[i] , height[j])* (j-i));
        //左指针指向元素    
        //短板必须动，长板直接忽略，找到更长的短板，其实把每一组解都遍历了
        if(height[i] < height[j])
        {
            i++ ;
        }
        else
        {
            j-- ;
        }
    }
    return res ;
}

int minCostClimbStairsDP(std::vector<int> &cost)
{
    int n = cost.size() - 1;
    if(n == 1 || n == 2 )
        return cost[n];
    // 初始化dp表 ，  用于存储子问题的解
    std::vector<int> dp( n + 1 );
    dp[1]  = cost[1] ;
    dp[2]  = cost[2] ;

    //状态转移 ： 从较小子问题逐步求解较大子问题
    for(int i = 3 ; i <= n ; i++)
    {
        dp[i] = min(dp[i-1] , dp[i-2])  + cost[i] ; 
    }
    return dp[n] ;
 }

 int minCostClimbingStairsDPComp(std::vector<int> &cost)
 {
    int n = cost.size() -1 ;
    if(n == 1 || n == 2 )
    {
        return cost[n] ; 
    }
    int a = cost[1] , b = cost[2] ;
    for(int i = 3 ; i <= n ; i++)
    {
        int tmp = b ;
        b = min(a,tmp) + cost[i];
        a = tmp;
    }
    return b ;
 } 

 /* 带约束爬楼梯：动态规划
 拓展状态定义[i,j],表示处在第i阶并且上一轮跳了j阶
 */
int climbingStairsConstraintDP(int n) {
    if (n == 1 || n == 2) {
        return 1;
    }
    // 初始化 dp 表，用于存储子问题的解
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(3, 0));
    // 初始状态：预设最小子问题的解
    dp[1][1] = 1;
    dp[1][2] = 0;
    dp[2][1] = 0;
    dp[2][2] = 1;
    // 状态转移：从较小子问题逐步求解较大子问题
    for (int i = 3; i <= n; i++) {
        dp[i][1] = dp[i - 1][2];
        dp[i][2] = dp[i - 2][1] + dp[i - 2][2];
    }
    return dp[n][1] + dp[n][2];
}