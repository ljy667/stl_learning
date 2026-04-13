#include <vector>
#include <iostream>

//1572 矩阵对焦元素之和，给你一个正方形mat，请你返回矩阵对角线元素之和
//输入 mat = [[1,2,3],[4,5,6],[7,8,9]]，输出 25（1+5+9+3+7）

class Solution{
    public:
    int diagonalsum(std::vector<std::vector<int>>& mat)
    {
        int ans=0;
        int size = mat.size();

        for(int i=0; i<size;i++)
        {
            ans +=mat[i][i];
            ans+= mat[i][size-i-1];
        }
        if( size%2 != 0)
        {
            ans -=mat[size/2][size/2];

        }
        return ans;
    }
};



int main()
{
    std::vector<std::vector<int>> mat = {{1,2,3},{4,5,6},{7,8,9}};
    Solution sol;
    int ans = sol.diagonalsum(mat);

    std::cout<<"ans:"<<ans<<std::endl;
}