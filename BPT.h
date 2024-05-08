//
// Created by 13199 on 2024/5/7.
//

#ifndef TICKETSYSTEM_BPT_H
#define TICKETSYSTEM_BPT_H

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cstring>

#define M 5
#define L 5

template<class INDEX, class VALUE>
class BPT {
private:
  size_t root; // towards the pos of root in file NODE
  std::fstream Node;
  std::fstream Info;

  size_t head; // the farthest ptr stored in NODE

private:
  struct node {
    INDEX keys[M + 1];
    int sons[M + 1];
    int size;
    bool type; // 0 node, 1 leaf
  };

  void initialize() {
    Node.open("NODE");
    if (!Node.good()) {
      Node.open("NODE", std::ios::out);
      Node.close();
      Node.open("NODE"); // create file

      head = 0;
      Node.seekp(0, std::ios::beg);
      Node.write(reinterpret_cast<char*>(&head), sizeof(size_t));
    }
    else {
      // todo whether empty, record if it's empty
      Node.seekg(0, std::ios::beg);
      Node.read(reinterpret_cast<char*>(&head), sizeof(size_t)); // read head
    }
    Node.close();

    Info.open("INFO");
    if (!Info.good()) {
      Info.open("INFO", std::ios::out | std::ios::binary);
      Info.close();
      Info.open("INFO");
      Info.seekp(0, std::ios::beg);
      int temp = 0;
      Info.write(reinterpret_cast<char*>(&temp), sizeof(int));
    }
    Info.close();

    // 当你使用close函数关闭文件时，会自动刷新缓冲区中的数据，
    // 确保数据被写入到文件中。而不关闭文件的话，
    // 缓冲区中的数据可能还未被写入文件，这样可能会导致数据丢失。
  }

  node readn(size_t pos) {
    // 读取一个node
    node result;
    Node.open("NODE");
    Node.seekg(pos, std::ios::beg);
    Node.read(reinterpret_cast<char*>(&result), sizeof(node));
    Node.close();
    return result;
  }

  std::pair<INDEX, VALUE>* readi(size_t pos) {
    // 读取一个数据块
    std::pair<INDEX, VALUE> result[L];
    Info.open("NODE");
    Info.seekg(pos, std::ios::beg);
    Info.read(reinterpret_cast<char*>(&result), sizeof(result)); // 存疑
    Info.close();
    return result;
  }

public:
  BPT() {

  }
  ~BPT() {

  }
}

#endif //TICKETSYSTEM_BPT_H
