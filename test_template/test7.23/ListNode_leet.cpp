#include <stdio.h>

int main(void)
{
    int a[5]={1,2,3,4,5};
    // int (*)[5] 表示一个指向数组的指针，该 类型的值不能直接用于初始化 int * 类型的实体
    // &a 的类型是 int (*)[5]，表示“指向一个包含 5 个 int 元素的数组的指针”。
    // &a + 1 表示在其基础上偏移了 一个数组的大小，结果仍然是 int (*)[5] 类型。
    int *ptr =(int*) (&a+1);
    printf("%d,%d",*(a+1),*(ptr-1));
    return 0;
}