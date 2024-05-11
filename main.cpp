#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include "BPT.h"

#define K 107

BPT<unsigned long long, int> s;

unsigned long long hash(std::string &s) {
  unsigned long long ans = 0;
  for (char c : s) {
    ans = ans * K + c;
  }
  return ans;
}

int main() {
//  freopen("in", "r", stdin);
//  freopen("out", "w", stdout);
  int n;
  std::cin >> n;
  std::string opt;
  std::string index;
  int value;
  for (int i = 0; i < n; ++i) {
    std::cin >> opt;
    if (opt == "insert") {
      std::cin >> index >> value;
      s.insert(hash(index), value);
    }
    else if (opt == "delete") {
      std::cin >> index >> value;
      s.erase(hash(index), value);
    }
    else if (opt == "find") {
      std::cin >> index;
      vector<int> ans = s.find(hash(index));
      if (!ans.empty()) for (int a : ans) {
        std::cout << a << " ";
      }
      else std::cout << "null";
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
