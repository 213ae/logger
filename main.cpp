#include <bits/stdc++.h>

#include "Logger.h"

struct MyType {
  [[nodiscard]] std::string str() const {
    return "MyType" + std::to_string(i);
  }
  int i = 0;
};

int main() {
  using std::operator""s;
  Logger log(&std::cout); 
  std::vector<double> a{1.1111111, 2.1111111, 3.1111111, 4.1111111};
  std::vector<double>& b = a;
  log(a);
  log(b);
  log(a, b);
  log(std::vector<int>{1, 2, 3, 4});
  log(std::set<int>{1, 2, 3, 4});
  log(std::deque<int>{1, 2, 3, 4});
  log(std::set<int>{1, 2, 3, 4});
  log(std::unordered_set<int>{1, 2, 3, 4});
  
  log << MyType(1) << MyType(2) << 'n';
  return 0;
}
