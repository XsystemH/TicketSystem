# Ticket System

SJTU CS1951 课程大作业

## BPlusTree

需要存储的数据: `index` and `value`
(使用模板类)

预设 `M = 5, L = 5`

    节点容量M = 4096字节 / ((8 + 4) * M + 4 + 4 + 20)
    解得 M ≈ 17.5

    节点容量L = 4096字节 / (8字节（键） + 4字节（值） + 8字节（指针） + 12字节（额外开销）)
    = 4096 / 32
    = 128

推荐 `M = 16, L = 128` (128 * (index + value) 略小于 4096字节 一次IO操作)

### Node

```c++
template<class INDEX>
struct node {
  INDEX keys[M];
  int sons[M];
  int size;
  int type;
};
```

### Info

```c++
template<class INDEX, class VALUE>
struct info {
  std::pair<INDEX, VALUE> vals[L];
  int size;
  int next
};
```

### 文件存储格式

`NODE`: |--根节点--|-------|-------|-------|

`INFO`: |-------|-------|-------|-------|

不需要计数器, 写入时直接移动到文件的终点, 并记录位置

### 文件初始化

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

### BPT成员

按理说需要的只有一个`root`的指针, 主要要做的就是文件创建、读取、初始化

```c++
template<class INDEX, class VALUE> 
class BPT {
  size_t root; // towards the pos of root in file NODE
  std::fstream Node;
  std::fstream Info;
  
  size_t head; // the farthest ptr stored in NODE
}
```

### 文件读写操作封装

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

info readi(size_t pos) {
  // 读取一个数据块
  info result[L];
  Info.open("INFO");
  Info.seekg(pos, std::ios::beg);
  Info.read(reinterpret_cast<char*>(&result), sizeof(result));
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

info writei(info i) {
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
```

只在开始和结束打开和关闭文件, 提高了效率

### 更新`root`

```c++
void renewroot(int newroot) {
  root = newroot;
  Node.open("NODE");
  Node.seekp(0, std::ios::beg);
  Node.write(reinterpret_cast<char*>(&root), sizeof(root));
  Node.close;
}
```

### Find操作

- step1. 找到所有可能包含`index`的leaf节点 (`findN`), 返回
- step2. 遍历 找到所有`index`对应的`value`(`findL`), 返回
- step3. 调用`findL`, 接受返回并输出. (`find`, public)

### Insert操作

路径记录 `vector<int>`

Question: 在可能存在多个相同`index`的情况下如何记录路径

Answer: DFS, 如果有缓存的话, 并不会有太大的重复回档的开销

当直接DFS没有找到时, 就说明应当在最后插入

插入的情况:

- case1: `root == 0` 即 树为空
  - step1. 创建新的节点, 标记为叶节点(孩子小于2)
  - step2. 创建数据块, 存入第一组键值对
  - step3. 更新节点的`keys` `sons`
  - step4. 存入外存, 将存储位置赋值给`root`,更新`root`
- case2: 目标数据块`size <  L`
  - step1. 更新数据块即可
- case3: 目标数据块`size == L`
  - step1. 分裂数据块
  - step2. 回溯父节点, 判断父节点是否满足`size < M`
    - 否, 分裂父节点(注意根节点), 重复step2
    - 是, 结束回溯
- case4. 需要分裂根节点
  - step1. 新建节点作为根节点
  - step2. 将原有根节点分裂为两个节点, 传递数据
  - step3. 更新根节点的索引

### delete操作

略

### BufferPool

```c++
  int  NodePos [PoolSize];
  node NodePool[PoolSize];
  int cur = 0, psi = 0;

  node readn(int pos) {
    // 读取一个node

    // 检测内存池
    int t = psi, i = cur - 1;
    while (t > 0) {
      if (i < 0) i += PoolSize;
      if (NodePos[i] == pos) {
        return NodePool[i];
      }
      t--, i--;
    }

    node result;
//    Node.open("NODE");
    Node.seekg(pos, std::ios::beg);
    Node.read(reinterpret_cast<char*>(&result), sizeof(node));
//    Node.close();

    if (psi == PoolSize) {
      Node.seekp(NodePos [cur], std::ios::beg);
      Node.write(reinterpret_cast<char*>(&NodePool[cur]), sizeof(node));
    } // 释放缓冲区 同步外存
    NodePos [cur] = pos;
    NodePool[cur] = result;
    cur = (cur + 1) % PoolSize;
    psi = std::min(psi + 1, PoolSize);

    return result;
  }

  void writen(int pos, node n) {
    // 检测内存池
    int t = psi, i = cur - 1;
    while (t > 0) {
      if (i < 0) i += PoolSize;
      if (NodePos[i] == pos) {
        NodePool[i] = n;
        return;
      }
      t--, i--;
    }
    //    Node.open("NODE");
    Node.seekp(pos, std::ios::beg);
    Node.write(reinterpret_cast<char*>(&n), sizeof(node));
    //    Node.close();
  }

  ~BPT() {
    for (int i = 0; i < psi; ++i) {
      Node.seekp(NodePos [i], std::ios::beg);
      Node.write(reinterpret_cast<char*>(&NodePool[i]), sizeof(node));
    } // 释放缓冲区 同步外存
    Node.close();
    Info.close();
  }
```

## TicketSystem后端

### 对于BPT的适应调整

由于需要存储用户、车次等多个信息, 需要支持传入两个文件名的构造函数

### User类

```c++
struct USERINFO {
  char username[20];
  char password[30];
  char name[5];
  char mailAddr[30];
  int  privilege;
}

class USER {
private:
  map<unsigned long long, int> LoginStack; // no need to store longer
  BPT<unsigned long long, USERINFO> Users("USER_NODE", "USER_INFO");
  
};
```

### Time&Date类
For the convenience

```c++
class TIME {
private:  
  int hh;
  int mon;
public:  
  operator int() const {
    return hh * 60 + mon;
  } // 计算时 统一到分钟 便于计算?
  friend std::ostream operator<<(std::ostream &o, TIME p) {
    os << std::setw(2) << std::setfill('0') << p.hh;
    os << '-';
    os << std::setw(2) << std::setfill('0') << p.mon;
    return os;
  }
  TIME(std::string &s) {
    hh = (s[0] - '0') * 10 + s[1] - '0';
    mon = (s[3] - '0') * 10 + s[4] - '0';
  }
};

class DATE {
private:
  int mon;
  int day;
pubulic:
  operator int() const {
    int ans = day;
    if (mon == 7) ans += 30;
    if (mon == 8) ans += 61;
    ans *= 60;
  } // 计算时 统一到分钟 便于计算?
  friend std::ostream operator<<(std::ostream &o, DATE p) {
    os << std::setw(2) << std::setfill('0') << p.mon;
    os << '-';
    os << std::setw(2) << std::setfill('0') << p.day;
    return os;
  }
  DATE(std::string &s) {
    mon = (s[0] - '0') * 10 + s[1] - '0';
    day = (s[3] - '0') * 10 + s[4] - '0';
  }
};
```

### Train类
```c++
struct TRAININFO_RAW {
  char trainID[20];
  int  stationNum;
  char stations[100][20];
  int  seatNum;
  int  prices[100];
  TIME startTime;
  int  travelTimes[100];
  int  stopoverTimes[100];
  DATE saleDate[2];
  char type;
};

struct TRAININFO {
  char trainID[20];
  int  stationNum;
  unsigned long long stations[100]; // after hash
  int  booked[100]; // 线段树 可便捷地查看区间余票
  int  leavingTime[100];
  int  arrivingTime[100];
};

class TRAIN {
  
};
```

### 指令处理

```c++
const std::string CMD[] = {
  "add_user", "login", "logout", "query_profile", "modify_profile", "add_train",
  "release_train", "query_train", "delete_train", "query_ticket", "query_transfer",
  "buy_ticket", "query_order", "refund_ticket", "clean", "exit", "query_privilege"
};
```

#### CMD类

```c++
class CMD {
pubulic:
  int timestamp;
  int type;
  int num;
  std::string cmd[26]; // c - 'a'
  
};
```

参数排序

### 数据存储

索引(仅储存ID和privilege):
乘客 车次

原始数据(线性?):
乘客信息
车次信息

处理数据
发布列车-精简信息（购票相关）
乘客-订单信息
列车-订单信息