//
// Created by 13199 on 2024/5/26.
//

#ifndef TICKETSYSTEM_STRUCTS_H
#define TICKETSYSTEM_STRUCTS_H

#include <iostream>
#include <string>
#include <iomanip>
#include "vector/vector.hpp"

using hashcode = unsigned long long;

const int K = 277;

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
    uid[other.len] = '\0';
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
    pwd[other.len] = '\0';
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
    name[other.len] = '\0';
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
    mail[other.len] = '\0';
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
//                               31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
const int day_of_month[13] = {0, 31, 60, 91,121,152,182,213,244,274,305,335,366};

class TIME {
private:
  int mon;
  int day;
  int hou;
  int min;
public:
  explicit operator int() const {
    int ans = day;
    ans += day_of_month[mon - 1];
    ans *= 24;
    ans += hou;
    ans *= 60;
    ans += min;
    return ans;
  } // 计算时 统一到分钟 便于比较
  std::string string_date() {
    std::string s;
    s = "00-00";
    s[0] = char(mon / 10 + '0');
    s[1] = char(mon % 10 + '0');
    s[3] = char(day / 10 + '0');
    s[4] = char(day % 10 + '0');
    return s;
  }
  std::string string_time() {
    std::string s;
    s = "00:00";
    s[0] = char(hou / 10 + '0');
    s[1] = char(hou % 10 + '0');
    s[3] = char(min / 10 + '0');
    s[4] = char(min % 10 + '0');
    return s;
  }
  TIME get_date() {
    TIME date;
    date.mon = mon;
    date.day = day;
    date.hou = 0;
    date.min = 0;
    return date;
  }
  TIME get_time() {
    TIME time;
    time.mon = 0;
    time.day = 0;
    time.hou = hou;
    time.min = min;
    return time;
  }
  TIME(std::string &s) {
    if (s[2] == '-') {
      mon = (s[0] - '0') * 10 + s[1] - '0';
      day = (s[3] - '0') * 10 + s[4] - '0';
      hou = 0;
      min = 0;
    }
    if (s[2] == ':') {
      mon = 0;
      day = 0;
      hou = (s[0] - '0') * 10 + s[1] - '0';
      min = (s[3] - '0') * 10 + s[4] - '0';
    }
  }
  TIME(int t) {
    hou = t / 60;
    min = t % 60;
    day = hou / 24; // 不超过72小时
    hou = hou % 24;
    for (int i = 1; i <= 12; ++i) {
      if (day_of_month[i - 1] < day <= day_of_month[i]) {
        mon = i;
        day -= day_of_month[i - 1];
      }
    }
  }
  TIME() {
    mon = 0;
    day = 0;
    hou = 0;
    min = 0;
  }
  friend bool operator<(const TIME& d1, const TIME& d2) {
    return int(d1) < int(d2);
  }
  TIME& operator++() {
    ++day;
    if ((day == 31 && mon == 6) || (day == 32 && mon == 7) || (day == 32 && mon == 8)) { // 最多9月2号
      ++mon;
      day = 1;
    }
    return *this;
  }
  friend TIME operator+(const TIME& t1, const TIME& t2) {
    TIME t;
    t.mon = t1.mon + t2.mon;
    t.day = t1.day + t2.day;
    t.hou = t1.hou + t2.hou;
    t.min = t1.min + t2.min;
    t.hou += t.min / 60;
    t.min %= 60;
    t.day += t.hou / 24;
    t.hou %= 60;
    if ((t.day >= 31 && t.mon == 6) || (t.day >= 32 && t.mon == 7) || (t.day == 32 && t.mon == 8)) {
      ++t.mon;
      t.day = 1;
    }
    return t;
  }
  friend int operator-(const TIME& t1, const TIME& t2) {
    return int(t1) - int(t2);
  }
  TIME& operator=(const TIME& other) = default;
  friend bool operator<(TIME& t1, TIME& t2) {
    return int(t1) < int(t2);
  }
  friend bool operator==(const TIME& t1, const TIME& t2) {
    return int(t1) == int(t2);
  }
  friend bool operator<=(TIME& t1, TIME& t2) {
    return int(t1) <= int(t2);
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
  hashcode get_hashcode() const {
    return h;
  }
  TIDTYPE& operator=(const TIDTYPE& other) {
    if (this == &other) return *this;
    for (int i = 0; i < other.len; ++i) {
      tid[i] = other.tid[i];
    }
    tid[other.len] = '\0';
    len = other.len;
    h = other.h;
    return *this;
  }
  friend bool operator==(TIDTYPE& tid1, TIDTYPE& tid2) {
    return tid1.get_hashcode() == tid2.get_hashcode();
  }
  friend bool operator<(const TIDTYPE& tid1, const TIDTYPE& tid2) {
    if (tid1.len != tid2.len) return tid1.len < tid2.len;
    for (int i = 0; i < tid1.len; ++i) {
      if (tid1.tid[i] != tid2.tid[i])
        return tid1.tid[i] < tid2.tid[i];
    }
    return false;
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
    sta[other.len] = '\0';
    len = other.len;
    h = other.h;
    return *this;
  }
  friend bool operator==(STATYPE& sta1, STATYPE& sta2) {
    return sta1.get_hashcode() == sta2.get_hashcode();
  }
  friend bool operator<(STATYPE& sta1, STATYPE& sta2) {
    return sta1.get_hashcode() < sta2.get_hashcode();
  }
  friend bool operator>(STATYPE& sta1, STATYPE& sta2) {
    return sta1.get_hashcode() > sta2.get_hashcode();
  }
};

struct TRAININFO_RAW {
  TIDTYPE trainID;
  int  stationNum;
  STATYPE stations[100];
  int  seatNum;
  int  prices[100];
  TIME startTime;
  int  travelTimes[100];
  int  stopoverTimes[100];
  TIME saleDate[2];
  char type;

  friend bool operator<(TRAININFO_RAW& t1, TRAININFO_RAW& t2) {
    return t1.trainID.get_hashcode() < t2.trainID.get_hashcode();
  }
  friend bool operator>(TRAININFO_RAW& t1, TRAININFO_RAW& t2) {
    return t1.trainID.get_hashcode() > t2.trainID.get_hashcode();
  }
  friend bool operator==(TRAININFO_RAW& t1, TRAININFO_RAW& t2) {
    return t1.trainID.get_hashcode() == t2.trainID.get_hashcode();
  }
};

// get when publish a train
// use to find transfer
struct TRAININFO {
  TIDTYPE trainID;
  int  stationNum;
  int  seatNum;
  STATYPE stations[100];
  int cost[100];
  TIME sDate, eDate;
  TIME arrivingTime[100]; // 0-0 based
  TIME leavingTime[100];
  char type;
  bool pub;

  TRAININFO(TRAININFO_RAW& t) {
    trainID = t.trainID;
    stationNum = t.stationNum;
    seatNum = t.seatNum;
    for (int i = 0; i < stationNum; ++i) {
      stations[i] = t.stations[i];
    }
    sDate = t.saleDate[0];
    eDate = t.saleDate[1];
    leavingTime[0] = t.startTime;
    cost[0] = 0;
    for (int i = 1; i < stationNum; ++i) {
      if (i > 1) leavingTime[i - 1] = arrivingTime[i - 1] + t.stopoverTimes[i - 1];
      arrivingTime[i] = leavingTime[i - 1] + t.travelTimes[i - 1];
      cost[i] = cost[i - 1] + t.prices[i - 1];
    }
    type = t.type;
    pub = false;
  }
  TRAININFO() = default;

  friend bool operator<(TRAININFO& t1, TRAININFO& t2) {
    return t1.trainID.get_hashcode() < t2.trainID.get_hashcode();
  }
  friend bool operator>(TRAININFO& t1, TRAININFO& t2) {
    return t1.trainID.get_hashcode() > t2.trainID.get_hashcode();
  }
  friend bool operator==(TRAININFO& t1, TRAININFO& t2) {
    return t1.trainID.get_hashcode() == t2.trainID.get_hashcode();
  }
};

struct STATION {
  TIDTYPE trainID;
  int cost; // from start station
  int rank; // n th in the road of the train
  TIME sDate, eDate;
  TIME arriving, leaving;

  STATION() = default;
  STATION(TRAININFO train, int i) {
    STATION sta;
    sta.trainID = train.trainID;
    sta.cost = train.cost[i];
    sta.rank = i;
    sta.sDate = train.sDate;
    sta.eDate = train.eDate;
    sta.arriving = train.arrivingTime[i];
    sta.leaving = train.leavingTime[i];
  }

  friend bool operator<(STATION& s1, STATION& s2) {
    return s1.trainID.get_hashcode() < s2.trainID.get_hashcode();
  }
  friend bool operator>(STATION& s1, STATION& s2) {
    return s1.trainID.get_hashcode() > s2.trainID.get_hashcode();
  }
  friend bool operator==(STATION& s1, STATION& s2) {
    return s1.trainID.get_hashcode() == s2.trainID.get_hashcode();
  }
};

// Seat of <DATE, Tid>
struct DAYTRAIN {
  int num;
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

  DAYTRAIN() =default;
  DAYTRAIN(int x) {
    num = x;
    for (int i = 0; i < 100; ++i) {
      seat[i] = x;
    }
  }

  friend bool operator<(DAYTRAIN& dt1, DAYTRAIN& dt2) {
    return dt1.num < dt2.num;
  }
  friend bool operator>(DAYTRAIN& dt1, DAYTRAIN& dt2) {
    return dt1.num > dt2.num;
  }
  friend bool operator==(DAYTRAIN& dt1, DAYTRAIN& dt2) {
    return dt1.num == dt2.num;
  }
};

struct TICKET {
  TIDTYPE trainID;
  STATYPE from;
  STATYPE dest;
  TIME leaving;
  TIME arriving;
  int cost;
  int seat;

  std::string show() {
    std::string s;
    s += std::string(trainID.tid);
    s += " " + std::string(from.sta);
    s += " " + leaving.string_date();
    s += " " + leaving.string_time();
    s += " ->";
    s += " " + std::string(dest.sta);
    s += " " + arriving.string_date();
    s += " " + arriving.string_time();
    s += " " + std::to_string(cost);
    s += " " + std::to_string(seat);
    return s;
  }
};

bool comp_time(const TICKET& t1, const TICKET& t2) {
  if ((t1.arriving - t1.leaving) != (t2.arriving - t2.leaving))
    return (t1.arriving - t1.leaving) < (t2.arriving - t2.leaving);
  return t1.trainID < t2.trainID;
};

bool comp_cost(const TICKET& t1, const TICKET& t2) {
  if (t1.cost != t2.cost) return t1.cost < t2.cost;
  return t1.trainID < t2.trainID;
}

std::string Status[3] = {"[success]", "[pending]", "[refunded]"};

struct ORDER {
  int rank;
  UIDTYPE userID;
  TIDTYPE trainID;
  TIME date; // 出发日期
  STATYPE from;
  int fr_rank;
  STATYPE dest;
  int to_rank;
  TIME leaving;
  TIME arriving;
  int cost;
  int seat;
  int status; // 0 success 1 pending 2 refunded

  std::string show() {
    std::string s;
    s += Status[status];
    s += " " + std::string(trainID.tid);
    s += " " + std::string(from.sta);
    s += " " + leaving.string_date();
    s += " " + leaving.string_time();
    s += " ->";
    s += " " + std::string(dest.sta);
    s += " " + arriving.string_date();
    s += " " + arriving.string_time();
    s += " " + std::to_string(cost);
    s += " " + std::to_string(seat);
    return s;
  }

  friend bool operator<(ORDER& o1, ORDER& o2) {
    return o1.rank < o2.rank;
  }
  friend bool operator>(ORDER& o1, ORDER& o2) {
    return o1.rank > o2.rank;
  }
  friend bool operator==(ORDER& o1, ORDER& o2) {
    return o1.rank == o2.rank;
  }
};

struct CMD {
  std::string timestamp;
  std::string type;
  int num = 0;
  std::string cmd[26]; // c - 'a'
};

#endif //TICKETSYSTEM_STRUCTS_H
