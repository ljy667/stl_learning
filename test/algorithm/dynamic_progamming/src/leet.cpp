#include <vector>
#include <queue>
#include <string>

struct ListNode
{
    int val;
    ListNode* next;

    // 三种构造函数
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* next) : val(x), next(next) {}

};


//链表两数相加
class solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2)
    {
        //开局创建dummy头， 储存求和的结果
        ListNode* dummy = new ListNode();
        ListNode* p = dummy ;  
        int c = 0 ;

        while(l1 && l2)
        {
            c += l1 -> val + l2 -> val;
            p -> next = new ListNode(c % 10);
            c /= 10 ; 
            p = p -> next , l1 = l1 -> next , l2 = l2 -> next ; 
        }

        while(l1)
        {
            c += l1 ->val ;
            p ->next = new ListNode(c % 10);
            c /= 10 ;
            p = p -> next , l1 = l1->next;
        }
        while (l2)
        {
            c += l2 ->val;
            p ->next = new ListNode(c % 10);
            c /= 10;
            p = p ->next , l2 ->next;
        }
        if (c) 
        {
            p -> next = new ListNode(c);
        }
        return dummy -> next ;
    }

};

//合并k个升序链表


ListNode* mergeKLists(std::vector<ListNode*>& lists)
{
    // 1. 最小堆：存储链表节点指针，自定义比较器Cmp
    priority_queue<ListNode* , std::vector<ListNode*> , Cmp> heap
    
    //把所有非空链表的头节点入堆，空链表直接跳过
    for(auto l:lists)  if (l) heap.push(l) ; 
    //辅助，串联新链表里所有节点
    ListNode* dummy = new ListNode(0) ;
    ListNode* tail = dummy ;

    //只要堆里还有东西，则还有待选节点

    //所有链表都经过堆这个中转站，挂到dummy头后面

    // 堆不为空，说明还有未拼接的节点
    while(heap.size()){
        auto t = heap.top();        // 取出当前最小值节点
        heap.pop();                 // 堆内删除该节点

        tail = tail -> next = t;

        // 如果当前节点还有下一个节点，放回堆参与后续比较
        if(t -> next)
            heap.push(t->next);  //把t->next元素放入k中比较，维持k中的元素为4个
    }
    return dummy -> next ;
}

//二分查找
int binarySearch(std::vector<int> &nums , int target)
{
    int i = 0 , j = nums.size() -1 ; 
    while( i <= j )
    {
        int m = i + (j-i)/2;
        if(nums[m] < target)
        {
            i = m + 1;
        }
        else if (nums[m] > target)
            j =  m -1 ;
        else 
            return  m ;
    }
    return -1 ;
}

int binarySearchInsertionSimple(std::vector<int>& nums , int target )
{
    int i = 0 , j = nums.size() - 1 ; 
    while(i <= j)
    {
        int m = i + (j-i) /2 ; //计算中点索引 m
        
        if(nums[m] < target){
            i = m + 1 ;
        }else if (nums[m] > target){
            j = m - 1;
        }
        else {
            return m ; 
        }
    }
    return i ;
}


//括号生成
{
    std::vector<std::string> ans ;
    std::vector<std::string> generateParenthesis(int n)
    {

    }
    
    /* 
    n：目标括号总对数
    lc：已使用的左括号数量
    rc：已使用的右括号数量
    seq：当前拼接的括号序列 
    */
    void dfs(int n , int lc , int rc , std::string seq)
    {
        //终止条件：左、右括号都用满n个 → 将合法序列存入结果ans
        if(lc == n && rc == n ){
            ans.push_back(seq);
            return;
        }
        //一对一对的添加
        if(lc < n)
            dfs( n, lc + 1 , rc , seq + '(');
        if(rc < n)
            dfs(n , lc , rc + 1 , seq + ')'); 
    }

}


/* 数字 n 代表生成括号的对数，请你设计一个函数，用于能够生成所有可能的并且 有效的 括号组合。
    示例 1：

    输入：n = 3
    输出：["((()))","(()())","(())()","()(())","()()()"]
    示例 2：

    输入：n = 1
    输出：["()"]

    「所有子集（任意长度，包含空集）」而不是全排列：总数是 (2^n)。
 */
 
class  soultion {
    bool valid(const std::string& str)
    {
        int balance = 0;
        for (char c:str)
        {
            if(c == '('){
                balance++;
            }else{
                balance--;
            }
            if(balance < 0)
            {
                return false;
            }
        }
        return balance == 0;
    }


    void generate_all(std::string& current , int n , std::vector<std::string>& result)
    {
        //当前str字符数量 == 2n
        if( n == current.size())
        {
            if(valid(current)){
                result.push_back(current);
            }
            return ;
        }
        current += '(' ;
        generate_all(current , n , result) ; 
        current.pop_back();
        current += ')' ;
        generate_all(current , n  , result);
        current.pop_back();
    }


    public:
        std::vector<std::string>  generateParenthesis(int n)
        {
            std::vector<std::string> result;
            std::string current;

            //括号组合数量必是2*n
            generate_all(current ,2*n , result);
            return result;
        }  



        /* 对给定的有序数组 nums 删除重复元素，在删除重复元素之后，每个元素只出现一次，并返回新的长度，
        上述操作必须通过原地修改数组的方法，使用 O(1) 的空间复杂度完成。

        由于给定的数组 nums 是有序的，因此对于任意 i<j，如果 nums[i]=nums[j]，
        则对任意 i≤k≤j，必有 nums[i]=nums[k]=nums[j]，即相等的元素在数组中的下标一定是连续的。
        利用数组有序的特点，可以通过双指针的方法删除重复元素。 */


        int removeDuplicates(std::vector<int>& nums)
        {
            int n = nums.size();
            if (n == 0)
            {
                return 0 ;
            }
            int fast = 1 , slow = 1;
            while(fast < n)
            {
                if(nums[fast] != nums[fast - 1]){
                    nums[slow] != nums[fast];
                    ++slow;
                }
                ++ fast ;
            }
            return slow;
        }

        /* 输入：haystack = "sadbutsad", needle = "sad"
            输出：0
            解释："sad" 在下标 0 和 6 处匹配。 */
        
        //暴力匹配
        int strStr(std::string haystack , std::string needle)
        {
            int n =  haystack.size()   , m = needle.size() ;
            for(int i =  0 ; i + m <= n ; i++ ){
                bool flag = true ; 
                for(int j = 0 ; j < m ; j++ ){
                    if(haystack[i+j] != needle[j]){
                        flag = false;
                        break;
                    }
                }
                if(flag){
                    return i;
                }
            }
            return -1;
        }


        /*  输入: dividend = 10, divisor = 3
            输出: 3
            解释: 10/3 = 3.33333.. ，向零截断后得到 3 */
        #include <climits>
        #include <algorithm>


    /*  我们需要将一个左边的「较小数」与一个右边的「较大数」交换，以能够让当前排列变大，从而得到下一个排列。
        同时我们要让这个「较小数」尽量靠右，而「较大数」尽可能小。
        当交换完成后，「较大数」右边的数需要按照升序重新排列。这样可以在保证新排列大于原来排列的情况下，使变大的幅度尽可能小。
        */

        void nextPermutation(std::vector<int>& nums)
        {
            int i = nums.size() -2 ;
            //向前找到一个 nums[i] < nums[i+1] 升序
            while ( i >= 0 && nums[i] >= nums[i+1]){
                i-- ; 
            }  
            if( i >= 0 ){
                int j = nums.size() - 1 ;  // 下标
                //向前找一个 nums[i] < nums[j]
                while( j >= 0  &&  nums[i] >= nums[j]){
                    j-- ;
                }
                std::swap(nums[i] , nums[j]);
            }
            std::reverse(nums.begin()+i+1 , nums.end());
        }

        //搜索旋转排序数组
        /*  输入：nums = [4,5,6,7,0,1,2], target = 0
            输出：4
            输入：nums = [4,5,6,7,0,1,2], target = 3
            输出：-1 
            输入：nums = [1], target = 0
            输出：-1

            但是这道题中，数组本身不是有序的，进行旋转后只保证了数组的局部是有序的，这还能进行二分查找吗？答案是可以的。
            可以发现的是，我们将数组从中间分开成左右两部分的时候，一定有一部分的数组是有序的。
            拿示例来看，我们从 6 这个位置分开以后数组变成了 [4, 5, 6] 和 [7, 0, 1, 2] 两个部分，其中左边 [4, 5, 6] 这个部分的数组是有序的，其他也是如此。
            */


            //逐个逼近
        int search(std::vector<int>& nums , int target)
        {
            int n = (int)nums.size();
            if(!n){
                return -1 ;
            }
            if(n == 1){
                return nums[0] == target ? 0 : -1 ;
            }
            int l = 0 , r = n - 1; //容器下标
            while( l <= r){
                int mid = (l+r)/2 ;
                if(nums[mid] == target) return mid;
                if(nums[0] <= nums[mid]){ // 左区间递增
                    if(nums[0] <= target && target < nums[mid]){ //在递增左区间里，右边界向前逼近
                        r = mid -1 ;    
                    }else{  //在右区间里 ，  右区间无序，左边界逐步向末尾逼近
                        l = mid+1 ;
                    }
                }else{  //右区间递增
                    if(nums[mid] < target && target <= nums[n-1]){  //在右区间里，左边界往右逼近
                        l = mid + 1;
                    }else{  //在左区间里，左区间无需，右边界逐步向队首递增
                        r = mid - 1;
                    }
                }
            } 
            return -1 ;
        }



        //再排序数组中查找元素的第一个和最后一个位置
        /* nums = [5,7,7,8,8,10], target = 8
        输出：[3,4]

        输入：nums = [5,7,7,8,8,10], target = 6
        输出：[-1,-1] 
        要找的是「第一个等于 target 的位置」（记为 leftIdx）和「第一个大于 target 的位置减一」（记为 rightIdx）。
        
        为了代码复用，如果 lower 为 true，则查找第一个大于等于 target 的下标，否则查找第一个大于 target 的下标
        */
        
        int binarySearch(std::vector<int>& nums , int target , bool lower)
        {
            int left = 0 , right = (int)nums.size() -1  , ans = (int)nums.size();
            while(left <= right)
            {
                int mid = (left + right) /2;
                if (nums[mid] > target || (lower && nums[mid] >= target)){
                    right = mid - 1 ;
                    ans = mid ;
                }else{
                    left = mid + 1;
                }
            }
            return ans ; 
        }

        std::vector<int> searchRange(std::vector<int>& nums , int target){
            int leftIdx = binarySearch(nums , target , true);        //左边界
            int rigthIdx = binarySearch(nums , target , false) -1 ;  //右边界

            if(leftIdx <= rigthIdx && rigthIdx < nums.size() && nums[leftIdx] == target && nums[rigthIdx] == target){
                return std::vector<int>{leftIdx , rigthIdx} ;
            }
            return std::vector<int> {-1 ,-1};
        }
        
        

        /*  输入: nums = [1,3,5,6], target = 5
            输出: 2

            输入: nums = [1,3,5,6], target = 2
            输出: 1

            输入: nums = [1,3,5,6], target = 7
            输出: 4     */

        int searchInsert(std::vector<int>& nums , int target)
        {
            int n = nums.size();
            int left = 0 ,right = n-1 ,ans = n;
            while(left <= right){  //二分法到最后一步，left = right , mid即为left的位置
                //得到的mid不一定是数组下标，所以要加上起点 `left`，才能得到中点位置。
                int mid = ((right - left) >>1 ) + left;
                // 等价于 int mid = left + (right - left)/2
                if(target <= nums[mid]){
                    ans = mid;
                    right = mid - 1;
                }else{
                    left = mid + 1;  
                }
            }
            return ans ;
        }


        

        //要压缩字符串 "3322251" ，
        //我们将 "33" 用 "23" 替换，将 "222" 用 "32" 替换，将 "5" 用 "15" 替换并将 "1" 用 "11" 替换。因此压缩后字符串变为 "23321511"。

        /*  - n=1 → `"1"`
            - n=2：读 n=1 的字符串：**1 个 1** → `"11"`
            - n=3：读 n=2 的字符串：**2 个 1** → `"21"`
            - n=4：读 n=3 的字符串：**1 个 2，1 个 1** → `"1211"`
            - n=5：读 n=4 的字符串：**1 个 1，1 个 2，2 个 1** → `"111221"` */

        std::string countAndSay(int n)
        {
            std::string prev =  "1" ;
            for(int i = 2 ; i <= n ; ++i){
                std::string curr = " " ;    // 
                int start = 0 ;         
                int  pos  = 0 ; 
                while(pos < prev.size()){
                    while(pos < prev.size() && prev[pos] == prev[start]){
                        pos++ ; 
                    }
                    curr += std::to_string(pos - start) + prev[start];
                    start = pos ;
                }
                prev = curr;
            }
            return prev; 
        }