#include <string> 
#include <iostream>
#include <stdint.h>
#include <string.h>
void Bsort(int* arr, size_t size)
{
    for(int i = 0 ; i < size-1; i++){
        for(int j=i ; j<size ; j++){
        if(arr[i] > arr[j])
            {
            int tmp = arr[i] ;
            arr[i] = arr [j] ;
            arr[j] = tmp;
            }
        }
    }
}
//own defind 

bool IsGreat(void *left, void* right ,size_t item_size ){
    //遍历每个字节比较
    for(int i =0 ; i< item_size ; i++){
        //偏移量 从高位开始比较，高位解引用获得值越大，left越大
        //https://www.doubao.com/thread/w2ade8e80a568790e
        //0x12345678 一个int , 等价于0x12 0x34 0x56 0x78  (4个16进制，32个位排列)
        //高位序  0x78 这个字节为16进制的最高位，因此有限比较最高位
        //*解引用， 比较最高位上存储的值
        if(*(static_cast<uint8_t*>(left) +item_size -i -1 )>
            *(static_cast<uint8_t*>(right) +item_size -i -1) ){
                return true ;
        } 
        else if (*(static_cast<uint8_t*>(left) + item_size - i -1) <
                *(static_cast<uint8_t *>(right) + item_size - i - 1))
                { return false;}
    }
    return false;
}

void Bsort(void* arr,size_t size, size_t item_size)
{
    for(int i = 0 ; i < size-1; i++){
        for(int j=i ; j<size ; j++){    
        
        //需要重构比较操作符
      if(IsGreat(static_cast<uint8_t*>(arr)+i*item_size , static_cast<uint8_t*>(arr)+ i*item_size , item_size))
      {
            uint8_t *tmp = static_cast<uint8_t*>(malloc(item_size));
            void *memcpy(tmp, static_cast<uint8_t*>(arr)+i*item_size, item_size);
            void *memcpy(static_cast<uint8_t*>(arr)+ i*item_size , static_cast<uint8_t*>(arr)+ j*item_size,item_size);
            void * memcpy(static_cast<uint8_t*>(arr)+ i*item_size , tmp ,item_size) ; 

      }
      
        // if(arr[i] > arr[j])
        //     {
        //     int tmp = arr[i] ;
        //     arr[i] = arr [j] ;
        //     arr[j] = tmp;
        //     }
        }
    }
}

void demo() 
{
    int arr[] {4,2,5,3,1};
    Bsort(arr,5, sizeof(int));
    for(auto item : arr)
    std::cout << item << std::endl;

}

int main(int argc,const char *argv[])
{
    demo();
}