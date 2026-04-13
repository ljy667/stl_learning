#include <iostream>
#include "dur_storage.h"

int update(int) {
    hero += 1; //将 hero 的值增加 1。
    return hero++; // 在返回 hero 的当前值后，再将 hero 的值增加 1。
}

void func() {
    double power = 2.2;
    std::cout << hero << std::endl;

    /*std::cout << ::power << std::endl;这行代码是打印全局的power变量，
    而double power = 2.2;是局部的power变量 */
    std::cout << ::power << std::endl;
}

int main()
{
    std::cout <<update(hero)<<std::endl;
    func();
    return 0;
}