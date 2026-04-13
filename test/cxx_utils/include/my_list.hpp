
#pragma once 
#include "List_Allocator.hpp"


//list 基本元素为 NODE,不是T（valuetype） 本身


//list 可能有非常复杂的内存分配机制
template<class T , class Alloc = MyAllocator<T>>
class MyList
{

};