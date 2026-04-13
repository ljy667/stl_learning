#include <reaction/reaction.h>
#include <iostream>
#include <iomanip>
#include <cmath>

int main() {
    using namespace reaction;

    // 1. 股票价格的响应式变量
    auto buyPrice = var(100.0).setName("buyPrice");      // 股票买入价
    auto currentPrice = var(105.0);                      // 当前市场价格

    // 2. 使用 'calc' 计算盈亏金额
    auto profit = calc([&]() {
        return currentPrice() - buyPrice();
    });

    // 3. 使用 'expr' 计算盈亏百分比
    auto profitPercent = expr(std::abs(currentPrice - buyPrice) / buyPrice * 100);

    // 4. 使用 'action' 在值变化时打印日志
    auto logger = action([&]() {
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "[股票更新] 当前价格: $" << currentPrice()
                  << ", 盈亏: $" << profit()
                  << " (" << profitPercent() << "%)\n";
    });

    // 模拟价格变动
    currentPrice.value(110.0).value(95.0);  // 股票价格上涨
    buyPrice.value(90.0);                   // 调整买入价

    return 0;
}


