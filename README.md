# Ticket System

SJTU CS1951 课程大作业

## BPlusTree

需要存储的数据: `index` and `value`
(使用模板类)

预设 `M = 5, L = 5`

### Node

```c++
template<class INDEX>
struct node {
  INDEX keys[M + 1];
  int sons[M + 1];
  int size;
  int type;
};
```

### Tree

按理说需要的只有一个`root`的指针, 主要要做的就是文件创建、读取、初始化

文件存储格式

`NODE`: |--计数器--|--根节点--|-------|-------|-------|

文件初始化

```c++
void initialize() { // in BPT
  Node.open();
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
  
  Info.open(INFO);
  if (!Info.good()) {
    Info.open(INFO, std::ios::out | std::ios::binary);
    Info.close();
    Info.open(INFO);
    Info.seekp(0, std::ios::beg);
    int temp = 0;
    Info.write(reinterpret_cast<char*>(&temp), sizeof(int));
  }
  Info.close();
  
  // 当你使用close函数关闭文件时，会自动刷新缓冲区中的数据，
  // 确保数据被写入到文件中。而不关闭文件的话，
  // 缓冲区中的数据可能还未被写入文件，这样可能会导致数据丢失。
}
```

节点

```c++
template<class INDEX, class VALUE> 
class BPT {
  size_t root; // towards the pos of root in file NODE
  std::fstream Node;
  std::fstream Info;
  
  size_t head; // the farthest ptr stored in NODE
}
```

文件读写操作封装

```c++
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
```