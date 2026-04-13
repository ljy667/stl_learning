
#include <concepts>
#include    <iostream>



template <typename T>
concept IsTrigMode = requires(T t) {
    { t.checkTrig() } -> std::same_as<bool>;
};

struct ChangeTrig {
    bool checkTrig() { return true; }
};

struct WrongTrig {
    int checkTrig() { return 1; }  // ❌ 返回 int，不是 bool
};

template<IsTrigMode TM>
void testTrig(TM t) {
    if (t.checkTrig()) {
        std::cout << "Triggered!\n";
    }
}

int main() {
    testTrig(ChangeTrig{}); // ✅ 编译成功
    //testTrig(WrongTrig{});  // ❌ 编译错误（不满足 IsTrigMode）
}
