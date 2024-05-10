//
// Created by 13199 on 2024/5/7.
//

#ifndef TICKETSYSTEM_BPT_H
#define TICKETSYSTEM_BPT_H

#include <iostream>
#include <vector> // 提交时记得换成“vector”
#include <fstream>
#include <string>
#include <cstring>

#include <iomanip> // 仅用于调试

#define vector std::vector
#define M 2
#define L 2

template<class INDEX, class VALUE>
class BPT {
private:
  int root; // towards the pos of root in file NODE
  std::fstream Node;
  std::fstream Info;

private:
  struct node {
    INDEX keys[M];
    int sons[M];
    int size = 0;
    bool type = false; // 0 node, 1 leaf
  };

  struct info {
    std::pair<INDEX, VALUE> vals[L];
    int size = 0;
  };

  void initialize() {
    Node.open("NODE");
    if (!Node.good()) {
      Node.open("NODE", std::ios::out);
      Node.close();
      Node.open("NODE"); // create file

      root = 0;
      Node.seekp(0, std::ios::beg);
      Node.write(reinterpret_cast<char*>(&root), sizeof(int));
    }
    else {
      Node.seekg(0, std::ios::beg);
      Node.read(reinterpret_cast<char*>(&root), sizeof(int)); // read head
    }
    Node.close();

    Info.open("INFO");
    if (!Info.good()) {
      Info.open("INFO", std::ios::out | std::ios::binary);
      Info.close();
      Info.open("INFO");
    }
    Info.close();

    // 当你使用close函数关闭文件时，会自动刷新缓冲区中的数据，
    // 确保数据被写入到文件中。而不关闭文件的话，
    // 缓冲区中的数据可能还未被写入文件，这样可能会导致数据丢失。
  }

  node readn(int pos) {
    // 读取一个node
    node result;
    Node.open("NODE");
    Node.seekg(pos, std::ios::beg);
    Node.read(reinterpret_cast<char*>(&result), sizeof(node));
    Node.close();
    return result;
  }

  info readi(int pos) {
    // 读取一整个数据块
    info result;
    Info.open("INFO");
    Info.seekg(pos, std::ios::beg);
    Info.read(reinterpret_cast<char*>(&result), sizeof(info));
    Info.close();
    return result;
  }

  int writen(node n) {
    Node.open("NODE");
    Node.seekp(0, std::ios::end);
    int ans = Node.tellp();
    Node.write(reinterpret_cast<char*>(&n), sizeof(node));
    Node.close();
    return ans;
  }

  int writei(info i) {
    Info.open("INFO");
    Info.seekp(0, std::ios::end);
    int ans = Info.tellp();
    Info.write(reinterpret_cast<char*>(&i), sizeof(info));
    Info.close();
    return ans;
  }

  void writen(int pos, node n) {
    Node.open("NODE");
    Node.seekp(pos, std::ios::beg);
    Node.write(reinterpret_cast<char*>(&n), sizeof(node));
    Node.close();
  }

  void writei(int pos, info i) {
    Info.open("INFO");
    Info.seekp(pos, std::ios::beg);
    Info.write(reinterpret_cast<char*>(&i), sizeof(info));
    Info.close();
  }

  void renewroot(int newroot) {
    root = newroot;
    Node.open("NODE");
    Node.seekp(0, std::ios::beg);
    Node.write(reinterpret_cast<char*>(&root), sizeof(root));
    Node.close();
  }

  void printN(node n) {
    std::cout << (n.type ? "leaf" : "node") << std::endl;
    std::cout << n.size;
    std::cout << "  ";
    int i;
    for(i = 0; i < n.size - 1; ++i) {
      std::cout << std::setw(4) << std::right << n.keys[i];
      std::cout << "|";
    }
    std::cout << std::setw(4) << std::right << n.keys[i];
    std::cout << std::endl;
    for(i = 0; i < n.size; ++i) {
      std::cout << std::setw(4) << std::right << n.sons[i];
      std::cout << "|";
    }
    std::cout << std::endl;
  }

  void printL(info l) {
    std::cout << l.size << std::endl;
    for (int i = 0; i < L; ++i) {
      std::cout << l.vals[i].first << " " << l.vals[i].second << std::endl;
    }
  }

  vector<int> findN(INDEX index) {
    vector<int> ans;
    vector<int> stack;
    int t = 0, i;
    stack.push_back(root);
    node n;
    while (t < stack.size()) {
      n = readn(stack[t]);
      ++t;
      bool flag = false;
      for (i = 0; i < n.size - 1; ++i) {
        if (n.keys[i] >= index) {
          if (n.keys[i] > index) {
            if (flag) break;
            else flag = true;
          }
          if (n.type) // leaf
            ans.push_back(n.sons[i]);
          else        // node
            stack.push_back(n.sons[i]);
        }
      }
      if (i == 0 || (i > 0 && n.keys[i - 1] <= index)) {
        if (n.type) // leaf
          ans.push_back(n.sons[i]);
        else        // node
          stack.push_back(n.sons[i]);
      }
    }
    return ans;
  }

  vector<VALUE> findL(INDEX index) {
    vector<VALUE> result;
    vector<int> poss = findN(index);
    for (int p : poss) {
      info in = readi(p);
      for (int i = 0; i < in.size; ++i) {
        if (in.vals[i].first == index)
          result.push_back(in.vals[i].second);
      }
    }
    return result;
  }

  int dfs(INDEX index, VALUE value, vector<std::pair<int, int>> &road) {
    node n = readn(road[road.size() - 1].second);
    bool flag = false;
    int i;
    for (i = 0; i < n.size - 1; ++i) {
      if (n.keys[i] >= index) {
        if (n.keys[i] > index) {
          if (flag) break;
          else flag = true;
        }
        if (n.type) {
          info in = readi(n.sons[i]);
          if (in.vals[0].first <= index) {
            if (in.vals[in.size - 1].first > index) return i;
            else if (in.vals[in.size - 1].first == index &&
                     in.vals[in.size - 1].second > value) return i;
          }
          else return -1;
        }
        else {
          road.push_back(std::make_pair(i,n.sons[i]));
          int ip = dfs(index, value, road);
          if (ip != -1) return ip;
          else road.pop_back();
        }
      }
    } // for
    if (i == 0 || (i > 0 && n.keys[i - 1] <= index)) { // one last
      if (n.type) {
        info in = readi(n.sons[i]);
        if (in.vals[0].first <= index) {
          if (in.vals[in.size - 1].first > index) return i;
          else if (in.vals[in.size - 1].first == index &&
                   in.vals[in.size - 1].second > value) return i;
        }
        else return -1;
      }
      else {
        road.push_back(std::make_pair(i,n.sons[i]));
        int ip = dfs(index, value, road);
        if (ip != -1) return ip;
        else road.pop_back();
      }
    }
    return -1;
  }

  void higher(int pos, node n1, node n2) {
    node nr;
    nr.size = 2;
    nr.type = false;
    nr.sons[0] = pos;
    nr.sons[1] = writen(n2);
    nr.keys[0] = readi(n2.sons[0]).vals[0].first;
    renewroot(writen(nr));
  }

  void insert_AUX(INDEX index, VALUE value) {
    // case1
    if (root == 0) {
      node n;
      n.size = 1;
      n.type = 1; // step 1
      info i;
      i.vals[i.size++] = std::make_pair(index, value); // 2
      n.sons[0] = writei(i);
      n.keys[0] = index; // 3
      renewroot(writen(n)); // 4
      return;
    }

    vector<std::pair<int, int>> road;
    road.push_back(std::make_pair(0, root));
    int ip = dfs(index, value, road);
    if (ip == -1) {
      node n = readn(root);
      while (!n.type) {
        road.push_back(std::make_pair(n.size - 1, n.sons[n.size - 1]));
        n = readn(n.sons[n.size - 1]);
      }
      ip = n.size - 1;
    }

    int ptr = road.size() - 1;

    node n = readn(road[ptr].second);
    info in = readi(n.sons[ip]);
    if (in.size < L) { // case2
      info ni;
      ni.size = in.size + 1;
      int t = in.size;
      bool flag = true;
      for (int i = in.size - 1; i >= 0; --i) {
        if (flag && (in.vals[i].first < index || (in.vals[i].first == index && in.vals[i].second < value))) {
          ni.vals[t--] = std::make_pair(index, value);
          flag = false; // 只插入一次
        }
        ni.vals[t--] = in.vals[i];
      }
      if (t == 0) ni.vals[0] = std::make_pair(index, value);
      writei(n.sons[ip], ni);
      return;
    }

    info i1, i2;
    int t = L - 1;
    bool flag = true;
    for (int i = L / 2 - 1; i >= 0; --i) {
      if (flag && (in.vals[t].first < index || (in.vals[t].first == index && in.vals[t].second < value))) {
        i2.vals[i] = std::make_pair(index, value);
        flag = false;
        continue;
      }
      i2.vals[i] = in.vals[t--];
    }
    i2.size = L / 2;
    for (int i = L / 2; i >= 0; --i) { // L is even
      if (flag && (in.vals[t].first < index || (in.vals[t].first == index && in.vals[t].second < value))) {
        i1.vals[i] = std::make_pair(index, value);
        flag = false;
        continue;
      }
      i1.vals[i] = in.vals[t--];
    }
    i1.size = L / 2 + 1;
    writei(n.sons[ip], i1);

    if (n.size < M) { // case3
      n.keys[n.size - 1] = i2.vals[0].first;
      n.sons[n.size++] = writei(i2);
      writen(road[ptr].second, n);
      return;
    }

    node n1, n2;
    t = M - 1;
    flag = true;
    for (int i = M / 2 - 1; i >= 0; --i) {
      if (flag && t == ip) {
        n2.sons[i] = writei(i2);
        if (i) n2.keys[i - 1] = i2.vals[0].first;
        n2.keys[i] = i2.vals[i2.size - 1].first;
        flag = false;
        continue;
      }
      n2.sons[i] = n.sons[t];
      if (i) n2.keys[i - 1] = n.keys[t - 1];
      n2.keys[i] = n.keys[t];
      t--;
    }
    n2.size = M / 2;
    n2.type = n.type;
    for (int i = M / 2; i >= 0; --i) {
      if (flag && t == ip) {
        n1.sons[i] = writei(i2);
        if (i) n1.keys[i - 1] = i2.vals[0].first;
        n1.keys[i] = i2.vals[i2.size - 1].first; ;
        flag = false;
        continue;
      }
      n1.sons[i] = n.sons[t];
      if (i) n1.keys[i - 1] = n.keys[t - 1];
      n1.keys[i] = n.keys[t];
      t--;
    }
    n1.size = M / 2 + 1;
    n1.type = n.type;
    writen(road[ptr].second, n1);

    if (road.size() > 1) ptr--; // road.pop_back();
    else {
      higher(road[ptr].second, n1, n2);
      ptr--;
    }

    while(ptr >= 0) {
      n = readn(road[ptr].second);
      if (n.size < M) { // case3
        node nn;
        nn.size = n.size + 1;
        t = n.size;
        for (int i = n.size - 1; i >= 0; --i) {
          if (i == road[ptr + 1].first) {
            nn.keys[t - 1] = readi(n2.sons[0]).vals[0].first;
            nn.sons[t] = writen(n2);
            t--;
          }
          if (i) nn.keys[t - 1] = n.keys[i - 1];
          nn.sons[t] = n.sons[i];
          t--;
        }
        writen(road[ptr].second, nn);
        return;
      }

      t = M - 1;
      flag = true;
      node temp = n2;
      for (int i = M / 2 - 1; i >= 0; --i) {
        if (flag && t == road[ptr + 1].first) {
          n2.sons[i] = writen(temp);
          if (i) n2.keys[i - 1] = readi(temp.sons[0]).vals[0].first;
          n2.keys[i] = readi(temp.sons[temp.size - 1]).vals[0].first;
          flag = false;
          continue;
        }
        n2.sons[i] = n.sons[t];
        if (i) n2.keys[i - 1] = n.keys[t - 1];
        n2.keys[i] = n.keys[t];
        t--;
      }
      n2.size = M / 2;
      n2.type = n.type;
      for (int i = M / 2; i >= 0; --i) {
        if (flag && t == road[ptr + 1].first) {
          n1.sons[i] = writen(temp);
          if (i) n1.keys[i - 1] = readi(temp.sons[0]).vals[0].first;
          n2.keys[i] = readi(temp.sons[temp.size - 1]).vals[0].first;
          flag = false;
          continue;
        }
        n1.sons[i] = n.sons[t];
        if (i) n1.keys[i - 1] = n.keys[t - 1];
        n1.keys[i] = n.keys[t];
        t--;
      }
      n1.size = M / 2 + 1;
      n1.type = n.type;
      writen(road[ptr].second, n1);

      if (road.size() > 1) ptr--; // road.pop_back();
      else {
        higher(road[ptr].second, n1, n2);
        ptr--; // road.pop_back();
      }
    }
  }

public:
  BPT() {
    initialize();
  }
  ~BPT() = default;

  void insert(INDEX index, VALUE value) {
    insert_AUX(index, value);
  }

  void erase(INDEX index, VALUE value) {
    // todo
  }

  vector<VALUE> find(INDEX index) {
    vector<VALUE> ans;
    if (root == 0) return ans; // root为0, 树为空
    ans = findL(index);
    return ans;
  }

  void test() {
    vector<int> ans;
    vector<int> stack;
    int t = 0, i;
    stack.push_back(root);
    node n;
    while (t < stack.size()) {
      n = readn(stack[t]);
      printN(n);
      ++t;
      for (i = 0; i < n.size; ++i) {
        if (n.type) // leaf
          ans.push_back(n.sons[i]);
        else        // node
          stack.push_back(n.sons[i]);
      }
    }
    for (int a : ans) {
      std::cout << "pos: " << a << std::endl;
      printL(readi(a));
    }
  }
};

#endif //TICKETSYSTEM_BPT_H
