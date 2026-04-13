#include <iostream>
#include <memory>
#include <type_traits>


struct SafeType{
    SafeType() noexcept =default;
    SafeType(const SafeType&) noexcept =default;
    SafeType& operator=(const SafeType&) noexcept =default;
};

struct MayThrow {
    MayThrow() {throw std::runtime_error("Oops in ctor!");}
    MayThrow& operator=(const MayThrow&){
        throw std::runtime_error("Oops in assign");
    }
};

//updateValue 泛型函数
template<typename Type, typename T>
bool updateValue(T&& t)

/* std::is_nothrow_constructible_v<Type, T&&>
→ 编译期常量，表示 Type 能否用 T&& 构造而且不抛异常。

std::is_nothrow_assignable_v<Type&, T&&>
→ 编译期常量，表示 Type& 能否被 T&& 赋值而且不抛异常。 */

noexcept(std::is_nothrow_constructible_v<Type,T>
     && std::is_nothrow_assignable_v<Type&,T>)
     {
        std::unique_ptr<Type> m_ptr;
        if(!m_ptr)
        {
            m_ptr =std::make_unique<Type>(std::forward<T>(t));
        }else{
            *m_ptr = std::forward<T>(t);
        }
        return true;
     }


int main(){
    std::cout << std::boolalpha;
    //检查SafeType 的noexcept 推导
    std::cout << "SafeType is_nothrow_constructible: "
              << std::is_nothrow_constructible_v<SafeType, SafeType&&> << "\n";
    std::cout << "SafeType is_nothrow_assignable: "
              << std::is_nothrow_assignable_v<SafeType&, SafeType&&> << "\n";
    std::cout << "updateValue<SafeType> noexcept? "
              << noexcept(updateValue<SafeType>(SafeType{})) << "\n\n";  

    // 检查 MayThrow 的 noexcept 推导
    std::cout << "MayThrow is_nothrow_constructible: "
              << std::is_nothrow_constructible_v<MayThrow, MayThrow&&> << "\n";
    std::cout << "MayThrow is_nothrow_assignable: "
              << std::is_nothrow_assignable_v<MayThrow&, MayThrow&&> << "\n";
    std::cout << "updateValue<MayThrow> noexcept? "
              << noexcept(updateValue<MayThrow>(MayThrow{})) << "\n";
}




