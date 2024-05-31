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

  void initialize() {
    Line.open(FILE);
    if (!Line.good()) {
      Line.open(FILE, std::ios::out);
      Line.close();
      Line.open(FILE); // create file
    }
  }
public:
  Linear() = default;
  ~Linear() {
    Line.close();
  }
  void select(std::string s) {
    FILE = std::move(s);
    initialize();
  }
  int insert(T t) {
    Line.seekp(0, std::ios::end);
    int ans = Line.tellp();
    Line.write(reinterpret_cast<char*>(&t), sizeof(T));
    return ans;
  }
  void insert(T t, int p) {
    Line.seekp(p, std::ios::beg);
    Line.write(reinterpret_cast<char*>(&t), sizeof(T));
  }
  T read(int p) {
    T result;
    Line.seekg(p, std::ios::beg);
    Line.read(reinterpret_cast<char*>(&result), sizeof(T));
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
  BBPT(std::string no, std::string in, std::string da) {
    database.select(da);
    mapping.select(no, in);
  }
  ~BBPT() = default;
  void insert(INDEX index, VALUE value) {
    mapping.insert(index, database.insert(value));
  }
  void erase(INDEX index, VALUE value) {
    vector<int> key = mapping.find(index);
    mapping.erase(index, key[0]);
  }
  void modify(INDEX index, VALUE value) {
    vector<int> key = mapping.find(index);
    database.insert(value, key[0]);
  }
  vector<VALUE> find(INDEX index) {
    vector<VALUE> ret;
    vector<int> keys = mapping.find(index);
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
