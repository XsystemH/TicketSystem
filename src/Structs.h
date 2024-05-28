//
// Created by 13199 on 2024/5/26.
//

#ifndef TICKETSYSTEM_STRUCTS_H
#define TICKETSYSTEM_STRUCTS_H

#include <iostream>
#include <string>
#include <iomanip>

#define K 277

using hashcode = unsigned long long;

struct UIDTYPE {
  char uid[20];
  int  len = 0;
  hashcode h = 0;

  UIDTYPE() = default;
  UIDTYPE(const std::string& s) {
    for (char c : s) {
      h = h * K + c;
      uid[len++] = c;
    }
  }
  hashcode get_hashcode() {
    return h;
  }
  UIDTYPE& operator=(const UIDTYPE& other) {
    if (this == &other) return *this;
    for (int i = 0; i < other.len; ++i) {
      uid[i] = other.uid[i];
    }
    len = other.len;
    h = other.h;
    return *this;
  }
  friend bool operator==(UIDTYPE& uid1, UIDTYPE& uid2) {
    return uid1.get_hashcode() == uid2.get_hashcode();
  }
  friend bool operator<(UIDTYPE& uid1, UIDTYPE& uid2) {
    return uid1.get_hashcode() < uid2.get_hashcode();
  }
  friend bool operator>(UIDTYPE& uid1, UIDTYPE& uid2) {
    return uid1.get_hashcode() > uid2.get_hashcode();
  }
};

struct PWDTYPE {
  char pwd[20];
  int  len = 0;
  hashcode h = 0;

  PWDTYPE() = default;
  PWDTYPE(const std::string& s) {
    for (char c : s) {
      h = h * K + c;
      pwd[len++] = c;
    }
  }
  hashcode get_hashcode() {
    return h;
  }
  PWDTYPE& operator=(const PWDTYPE& other) {
    if (this == &other) return *this;
    for (int i = 0; i < other.len; ++i) {
      pwd[i] = other.pwd[i];
    }
    len = other.len;
    h = other.h;
    return *this;
  }
  friend bool operator==(PWDTYPE& pwd1, PWDTYPE pwd2) {
    return pwd1.get_hashcode() == pwd2.get_hashcode();
  }
};

struct NAMETYPE {
  char name[10]; // 中文字符存贮
  int  len = 0;
  // 仅存贮字符信息 不涉及比较
  NAMETYPE() = default;
  NAMETYPE(const std::string& s) {
    for (char c : s) {
      name[len++] = c;
    }
  }
  NAMETYPE& operator=(const NAMETYPE& other) {
    if (this == &other) return *this;
    for (int i = 0; i < other.len; ++i) {
      name[i] = other.name[i];
    }
    len = other.len;
    return *this;
  }
};

struct MAILTYPE {
  char mail[30];
  int  len = 0;

  MAILTYPE() = default;
  MAILTYPE(const std::string& s) {
    for (char c : s) {
      mail[len++] = c;
    }
  }
  MAILTYPE& operator=(const MAILTYPE& other) {
    if (this == &other) return *this;
    for (int i = 0; i < other.len; ++i) {
      mail[i] = other.mail[i];
    }
    len = other.len;
    return *this;
  }
};

struct USERINFO {
  UIDTYPE username;
  PWDTYPE password;
  NAMETYPE name;
  MAILTYPE mailAddr;
  int  privilege;

  friend bool operator<(USERINFO& u1, USERINFO& u2) {
    return u1.username < u2.username;
  }
  friend bool operator>(USERINFO& u1, USERINFO& u2) {
    return u1.username > u2.username;
  }
  friend bool operator==(USERINFO& u1, USERINFO& u2) {
    return u1.username == u2.username;
  }
  std::string information() {
    std::string s;
    s = std::string(username.uid) + " ";
    s = s + std::string(name.name) + " ";
    s = s + std::string(mailAddr.mail) + " ";
    s = s + std::to_string(privilege);
    return s;
  }
};

class TIME {
private:
  int hh;
  int mm;
public:
  operator int() const {
    return hh * 60 + mm;
  } // 计算时 统一到分钟 便于计算?
  friend std::ostream& operator<<(std::ostream &os, TIME p) {
    os << std::setw(2) << std::setfill('0') << p.hh;
    os << '-';
    os << std::setw(2) << std::setfill('0') << p.mm;
    return os;
  }
  TIME(std::string &s) {
    hh = (s[0] - '0') * 10 + s[1] - '0';
    mm = (s[3] - '0') * 10 + s[4] - '0';
  }
  TIME() = default;
};

class DATE {
private:
  int mm;
  int dd;
public:
  explicit operator int() const {
    int ans = dd;
    if (mm == 7) ans += 30;
    if (mm == 8) ans += 61;
    ans *= 60;
    return ans;
  } // 计算时 统一到分钟 便于计算?
  friend std::ostream& operator<<(std::ostream &os, DATE p) {
    os << std::setw(2) << std::setfill('0') << p.mm;
    os << '-';
    os << std::setw(2) << std::setfill('0') << p.dd;
    return os;
  }
  DATE(std::string &s) {
    mm = (s[0] - '0') * 10 + s[1] - '0';
    dd = (s[3] - '0') * 10 + s[4] - '0';
  }
  DATE() = default;
  friend bool operator<(const DATE& d1, const DATE& d2) {
    return int(d1) < int(d2);
  }
};

struct TIDTYPE {
  char tid[20];
  int  len = 0;
  hashcode h = 0;

  TIDTYPE() = default;
  TIDTYPE(const std::string& s) {
    for (char c : s) {
      h = h * K + c;
      tid[len++] = c;
    }
  }
  hashcode get_hashcode() {
    return h;
  }
  TIDTYPE& operator=(const TIDTYPE& other) {
    if (this == &other) return *this;
    for (int i = 0; i < other.len; ++i) {
      tid[i] = other.tid[i];
    }
    len = other.len;
    h = other.h;
    return *this;
  }
  friend bool operator==(TIDTYPE& tid1, TIDTYPE& tid2) {
    return tid1.get_hashcode() == tid2.get_hashcode();
  }
};

struct STATYPE {
  char sta[20];
  int  len = 0;
  hashcode h = 0;

  STATYPE() = default;
  STATYPE(const std::string& s) {
    for (char c : s) {
      h = h * K + c;
      sta[len++] = c;
    }
  }
  hashcode get_hashcode() {
    return h;
  }
  STATYPE& operator=(const STATYPE& other) {
    if (this == &other) return *this;
    for (int i = 0; i < other.len; ++i) {
      sta[i] = other.sta[i];
    }
    len = other.len;
    h = other.h;
    return *this;
  }
  friend bool operator==(STATYPE& sta1, STATYPE& sta2) {
    return sta1.get_hashcode() == sta2.get_hashcode();
  }
};

struct TRAININFO_RAW {
  TIDTYPE trainID;
  int  stationNum;
  char stations[100];
  int  seatNum;
  int  prices[100];
  TIME startTime;
  int  travleTimes[100];
  int  stopoverTimes[100];
  DATE saleDate[2];
  char type;
};

struct DAYTRAIN {
  int seat[100];

  int query_seat(int l, int r) {
    int ans = INT32_MAX;
    for (int i = l; i < r; ++i)
      ans = std::min(ans, seat[i]);
    return ans;
  }

  void modify_seat(int l, int r, int a) {
    for (int i = l; i < r; ++i)
      seat[i] += a;
  }
};

// get when publish a train
struct TRAININFO {
  TIDTYPE trainID;
  int  stationNum;
  hashcode stations[100];
  int  leavingTime[100];
  int  arrivingTime[100];
};

struct ORDER {

};

struct CMD {
  std::string timestamp;
  std::string type;
  int num = 0;
  std::string cmd[26]; // c - 'a'
};

#endif //TICKETSYSTEM_STRUCTS_H
