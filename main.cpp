#include <iostream>
#include <string>
#include <vector>
#include "BPT.h"

BPT<int, int> s;

int main() {
  int n;
  std::cin >> n;
  std::string opt;
  int index, value;
  for (int i = 0; i < n; ++i) {
    std::cin >> opt;
    if (opt == "insert") {
      std::cin >> index >> value;
      s.insert(index, value);
    }
    else if (opt == "delete") {
      std::cin >> index >> value;
      s.erase(index, value);
    }
    else if (opt == "find") {
      std::cin >> index;
      vector<int> ans = s.find(index);
      for (int a : ans) {
        std::cout << a << " ";
      }
      std::cout << std::endl;
    }
    else if (opt == "test") {
      s.test();
    }
    else if (opt == "clear") {
      system("rm NODE");
      system("rm INFO");
      return 0;
    }
  }
  return 0;
}
