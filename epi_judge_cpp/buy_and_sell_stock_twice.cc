#include <vector>
#include <iostream>
#include "test_framework/generic_test.h"
using std::vector;
double BuyAndSellStockTwice(const vector<double>& prices) {
    if (prices.empty()) return 0;

    vector<double> forward_pass(prices.size(), 0);
    double min_price = prices[0];
    for (int i = 1; i < prices.size(); ++i) {
        forward_pass[i] = std::max(prices[i] - min_price, forward_pass[i-1]);
        min_price = std::min(min_price, prices[i]);
    }
    double max_price = prices.back(), max_profit = 0, total_profit = forward_pass.back();
    for (int i = prices.size()-2; i >= 0; --i) {
        max_price = std::max(max_price, prices[i+1]);
        max_profit = std::max(max_profit, max_price - prices[i+1]);
        total_profit = std::max(forward_pass[i] + max_profit, total_profit);
    }

    return total_profit;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"prices"};
  return GenericTestMain(args, "buy_and_sell_stock_twice.cc",
                         "buy_and_sell_stock_twice.tsv", &BuyAndSellStockTwice,
                         DefaultComparator{}, param_names);
}
