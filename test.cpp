//
// Created by 13199 on 2024/5/11.
//
#include <iostream>
#include <cstdio>
#include <time.h>
#include <cstdlib>

bool m[32][32];

int main() {
  freopen("testcase3.in", "w", stdout);
  srand(time(0));
  int n = rand()%1000;
  std::cout << n << std::endl;
  for (int i = 0; i < n; ++i) {
    int op = rand()%8;
    if (i==0||op > 3) { // 75% insert
      int index = rand()%32, value = rand()%32;
      while (m[index][value]) {
        index = rand()%32, value = rand()%32;
      }
      m[index][value] = true;
      std::cout << "insert " << index << " " << value << std::endl;
    }
    else if (op) {
      int index = rand()%10, value = rand()%32;
      int t = 0;
      while (!m[index][value] && t < 32 * 32) {
        index = rand()%32, value = rand()%32;
        t++;
      }
      m[index][value] = false;
      std::cout << "delete " << index << " " << value << std::endl;
    }
    else {
      int index = rand()%32;
      std::cout << "find " << index << std::endl;
      bool flag = true;
      for (int j = 0; j < 32; ++j) {
        if (m[index][j]) {
          flag = false;
          std::cerr << j << " ";
        }
      }
      if (flag) std::cerr << "null";
      std::cerr << std::endl;
    }
  }
  return 0;
}