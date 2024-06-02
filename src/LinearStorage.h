//
// Created by 13199 on 2024/5/30.
//

#ifndef TICKETSYSTEM_LINEARSTORAGE_H
#define TICKETSYSTEM_LINEARSTORAGE_H

#include <iostream>
#include <fstream>
#include <utility>
#include "BPT.h"
#include "vector/vector.hpp"

template<class T>
class Linear {
private:
  std::string FILE;
  std::fstream Line;

  int  NodePos [PoolSize];
  T NodePool[PoolSize];
  int cur = 0, psi = 0;

  void initialize() {
    Line.open(FILE);
    if (!Line.good()) {
      Line.open(FILE, std::ios::out | std::ios::binary);
      Line.close();
      Line.open(FILE); // create file
    }
  }
public:
  Linear() = default;
  Linear(std::string s) {
    FILE = s;
    initialize();
  }
  ~Linear() {
    for (int i = 0; i < psi; ++i) {
      Line.seekp(NodePos [i], std::ios::beg);
      Line.write(reinterpret_cast<char*>(&NodePool[i]), sizeof(T));
    }
    Line.close();
  }
  int insert(T t) {
    Line.seekp(0, std::ios::end);
    int ans = Line.tellp();
    Line.write(reinterpret_cast<char*>(&t), sizeof(T));
    return ans;
  }
  void insert(T t, int p) {
    int x = psi, i = cur - 1;
    while (x > 0) {
      if (i < 0) i += PoolSize;
      if (NodePos[i] == p) {
        NodePool[i] = t;
        return;
      }
      x--, i--;
    }

    Line.seekp(p, std::ios::beg);
    Line.write(reinterpret_cast<char*>(&t), sizeof(T));
  }
  T read(int p) {
    int t = psi, i = cur - 1;
    while (t > 0) {
      if (i < 0) i += PoolSize;
      if (NodePos[i] == p) {
        return NodePool[i];
      }
      t--, i--;
    }

    T result;
    Line.seekg(p, std::ios::beg);
    Line.read(reinterpret_cast<char*>(&result), sizeof(T));

    if (psi == PoolSize) {
      Line.seekp(NodePos [cur], std::ios::beg);
      Line.write(reinterpret_cast<char*>(&NodePool[cur]), sizeof(T));
    } // 释放缓冲区 同步外存
    NodePos [cur] = p;
    NodePool[cur] = result;
    cur = (cur + 1) % PoolSize;
    psi = std::min(psi + 1, PoolSize);

    return result;
  }
  int size() {
    Line.seekp(0, std::ios::end);
    return Line.tellp() / sizeof(T);
  }
  void clear() {
    Line.close();
    std::string cmd = "rm " + FILE;
    int result = system(cmd.c_str());
    initialize();
  }
};

template<class INDEX, class VALUE>
class BBPT { // better BPT
private:
  Linear<VALUE> database;
  BPT<INDEX, int> mapping;
public:
  BBPT(std::string no, std::string in, std::string da) : database(da), mapping(no, in) {}
  ~BBPT() = default;
  void insert(INDEX index, VALUE value) {
    int p = database.insert(value);
    mapping.insert(index, p);
  }
  void erase(INDEX index, VALUE value) {
    vector<int> key = mapping.find(index);
    mapping.erase(index, key[0]);
  }
  void modify(INDEX index, VALUE value) {
    vector<int> key = mapping.find(index);
    if (key.size() == 1) {
      database.insert(value, key[0]);
      return;
    }
  }
  void modify_(INDEX index, VALUE value) {
    vector<int> key = mapping.find(index);
    if (key.size() == 1) {
      database.insert(value, key[0]);
      return;
    }
    for (int i = 0; i < key.size(); ++i) {
      if (database.read(key[i]) == value) {
        database.insert(value, key[i]);
        return;
      }
    }
  }
  void modify(INDEX index, VALUE value, int n) {
    vector<int> key = mapping.find(index);
    database.insert(value, key[n]);
  }
  vector<VALUE> find(INDEX index) {
    vector<VALUE> ret;
    vector<int> keys = mapping.find(index);
    if (keys.empty()) return ret;
    for (int i : keys) {
      ret.push_back(database.read(i));
    }
    return ret;
  }
  bool empty() {
    return mapping.empty();
  }
  int size() {
    return database.size();
  }
  void clear() {
    mapping.clear();
    database.clear();
  }
};

#endif //TICKETSYSTEM_LINEARSTORAGE_H
