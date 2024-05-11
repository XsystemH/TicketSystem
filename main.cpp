#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include "BPT.h"

#define K 277

class MyString {
public:
  char a[64];
  int len;

public:
  MyString() {
    len = 0;
  }
  MyString(std::string s) {
    for (char c : s) {
      a[len++] = c;
    }
  }
  ~MyString() = default;

  bool operator<(const MyString &other) const {
    if (len != other.len) return len < other.len;
    for (int i = 0; i < len; ++i) {
      if (a[i] != other.a[i]) return a[i] < other.a[i];
    }
    return false;
  }

  bool operator==(const MyString &other) const {
    if (len != other.len) return false;
    for (int i = 0; i < len; ++i) {
      if (a[i] != other.a[i]) return false;
    }
    return true;
  }

  bool operator>=(const MyString &other) const {
    return !(*this < other);
  }

  bool operator<=(const MyString &other) const {
    return (*this < other) || (*this == other);
  }

  bool operator>(const MyString &other) const {
    return !(*this <= other);
  }

  friend std::ostream& operator<<(std::ostream& os, const MyString& s) {
    for(int i = 0; i < s.len; ++i) {
      os << s.a[i];
    }
    return os;
  }
};

BPT<MyString, int> s;

unsigned long long hash(std::string &s) {
  unsigned long long ans = 0;
  for (char c : s) {
    ans = ans * K + c + 129;
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
      s.insert(MyString(index), value);
    }
    else if (opt == "delete") {
      std::cin >> index >> value;
      s.erase(MyString(index), value);
    }
    else if (opt == "find") {
      std::cin >> index;
      vector<int> ans = s.find(MyString(index));
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
