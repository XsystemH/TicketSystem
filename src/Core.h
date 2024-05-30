//
// Created by 13199 on 2024/5/28.
//

#ifndef TICKETSYSTEM_CORE_H
#define TICKETSYSTEM_CORE_H

#include "vector/vector.hpp"
#include "Storage.h"
#include "MergeSort.h"

int to_num(const std::string& s) {
  int ans = 0;
  for (char c : s) {
    ans = ans * 10 + c - '0';
  }
  return ans;
}

hashcode get_hashcode(const std::string& s) {
  hashcode h = 0;
  for (char c : s) {
    h = h * K + c;
  }
  return h;
}

// User

std::string add_user(const CMD& cmd) {
  if (Users.empty()) {
    USERINFO new_user;
    new_user.username = cmd.cmd['u' - 'a'];
    new_user.password = cmd.cmd['p' - 'a'];
    new_user.name = cmd.cmd['n' - 'a'];
    new_user.mailAddr = cmd.cmd['m' - 'a'];
    new_user.privilege = 10;

    Users.insert(new_user.username.get_hashcode(), new_user);
    return "0";
  }

  hashcode cur = UIDTYPE(cmd.cmd['c' - 'a']).get_hashcode();
  if (LoginStack.find(cur) == LoginStack.end()) return "-1";
  // -c 已登录
  if (LoginStack[cur] <= to_num(cmd.cmd['g' - 'a'])) return "-1";
  // 新用户的权限低于 -c 的权限

  USERINFO new_user;
  new_user.username = cmd.cmd['u' - 'a'];
  if (!Users.find(new_user.username.get_hashcode()).empty()) return "-1";
  // 如果 <username> 已经存在则注册失败
  new_user.password = cmd.cmd['p' - 'a'];
  new_user.name = cmd.cmd['n' - 'a'];
  new_user.mailAddr = cmd.cmd['m' - 'a'];
  new_user.privilege = to_num(cmd.cmd['g' - 'a']);

  Users.insert(new_user.username.get_hashcode(), new_user);
  return "0";
}

std::string login(CMD& cmd) {
  vector<USERINFO> user = Users.find(get_hashcode(cmd.cmd['u' - 'a']));
  if (user.empty()) return "-1";
  // 用户不存在
  if (!(user[0].password == PWDTYPE(cmd.cmd['p' - 'a']))) return "-1";
  // 密码错误
  if (LoginStack.find(get_hashcode(cmd.cmd['u' - 'a'])) != LoginStack.end()) return "-1";
  // 用户已经登录
  LoginStack.insert(sjtu::pair(user[0].username.get_hashcode(), user[0].privilege));
  return "0";
}

std::string logout(CMD& cmd) {
  if (LoginStack.find(get_hashcode(cmd.cmd['u' - 'a'])) == LoginStack.end()) return "-1";
  LoginStack.erase(get_hashcode(cmd.cmd['u' - 'a']));
  return "0";
}

std::string query_profile(CMD& cmd) {
  hashcode cur = UIDTYPE(cmd.cmd['c' - 'a']).get_hashcode();
  if (LoginStack.find(cur) == LoginStack.end()) return "-1";
  // -c 已登录
  vector<USERINFO> user = Users.find(get_hashcode(cmd.cmd['u' - 'a']));
  if (user.empty()) return "-1";
  // -u 存在
  if (LoginStack[cur] < user[0].privilege) return "-1";
  if (LoginStack[cur] == user[0].privilege && cur != user[0].username.get_hashcode()) return "-1";
  // -c 的权限大于 -u 的权限，或是 -c 和 -u 相同
  return user[0].information();
}

std::string modify_profile(CMD& cmd) {
  hashcode cur = UIDTYPE(cmd.cmd['c' - 'a']).get_hashcode();
  if (LoginStack.find(cur) == LoginStack.end()) return "-1";
  // -c 已登录
  vector<USERINFO> user = Users.find(get_hashcode(cmd.cmd['u' - 'a']));
  if (user.empty()) return "-1";
  // -u 存在
  if (LoginStack[cur] < user[0].privilege) return "-1";
  if (LoginStack[cur] == user[0].privilege && cur != user[0].username.get_hashcode()) return "-1";
  // -c 的权限大于 -u 的权限，或是 -c 和 -u 相同

  Users.erase(user[0].username.get_hashcode(), user[0]);

  if (!cmd.cmd['p' - 'a'].empty()) user[0].password = cmd.cmd['p' - 'a'];
  if (!cmd.cmd['n' - 'a'].empty()) user[0].name = cmd.cmd['n' - 'a'];
  if (!cmd.cmd['m' - 'a'].empty()) user[0].mailAddr = cmd.cmd['m' - 'a'];
  if (!cmd.cmd['g' - 'a'].empty()) user[0].privilege = to_num(cmd.cmd['p' - 'a']);

  Users.insert(user[0].username.get_hashcode(), user[0]);
  if (LoginStack.find(user[0].username.get_hashcode()) != LoginStack.end())
    LoginStack[user[0].username.get_hashcode()] = user[0].privilege;
  return user[0].information();
}

// Train

vector<std::string> devide_string(std::string s) {
  for (char &c : s) {
    if (c == '|') c = ' ';
  }
  std::istringstream is(s);
  vector<std::string> ans;
  std::string t;
  while (is >> t) {
    ans.push_back(t);
  }
  return ans;
}

vector<int> devide_num(std::string s) {
  for (char &c : s) {
    if (c == '|') c = ' ';
  }
  std::istringstream is(s);
  vector<int> ans;
  int t;
  while (is >> t) {
    ans.push_back(t);
  }
  return ans;
}

std::string add_train(CMD& cmd) {
  if (!Trains.find(get_hashcode(cmd.cmd['i' - 'a'])).empty()) return "-1";

  TRAININFO_RAW new_train;
  new_train.trainID = cmd.cmd['i' - 'a'];
  new_train.stationNum = to_num(cmd.cmd['n' - 'a']);
  new_train.seatNum = to_num(cmd.cmd['m' - 'a']);
  vector<std::string> sName = devide_string(cmd.cmd['s' - 'a']);
  for (int i = 0; i < sName.size(); ++i) {
    new_train.stations[i] = sName[i];
    new_train.stationNum++;
  }
  vector<int> p = devide_num(cmd.cmd['p' - 'a']);
  for (int i = 0; i < p.size(); ++i) {
    new_train.prices[i] = p[i];
  }
  new_train.startTime = cmd.cmd['x' - 'a'];
  vector<int> tTime = devide_num(cmd.cmd['t' - 'a']);
  for (int i = 0; i < tTime.size(); ++i) {
    new_train.travleTimes[i] = tTime[i];
  }
  vector<int> sTime = devide_num(cmd.cmd['o' - 'a']);
  for (int i = 0; i < sTime.size(); ++i) {
    new_train.stopoverTimes[i] = sTime[i];
  }
  vector<std::string> sDate = devide_string(cmd.cmd['d' - 'a']);
  new_train.saleDate[0] = sDate[0];
  new_train.saleDate[1] = sDate[0];
  new_train.type = cmd.cmd['y' - 'a'][0];

  Trains.insert(new_train.trainID.get_hashcode(), new_train);
  return "0";
}

std::string delete_train(CMD& cmd) {
  vector<TRAININFO> train = Trains.find(get_hashcode(cmd.cmd['i' - 'a']));
  if (train.empty()) return "-1";
  if (train[0].pub)  return "-1";

  Trains.erase(get_hashcode(cmd.cmd['i' - 'a']), train[0]);
  return "0";
}

std::string release_train(CMD& cmd) {
  vector<TRAININFO> train = Trains.find(get_hashcode(cmd.cmd['i' - 'a']));
  if (train.empty()) return "-1";

  Trains.erase(train[0].trainID.get_hashcode(), train[0]);
  train[0].pub = true;
  Trains.insert(train[0].trainID.get_hashcode(), train[0]);
  // pub
  for (int i = 0; i < train[0].stationNum; ++i) {
    STATION sta;
    sta.trainID = train[0].trainID;
    sta.cost = train[0].cost[i];
    sta.rank = i;
    sta.seatNum = train[0].seatNum;
    sta.sDate = train[0].sDate;
    sta.eDate = train[0].eDate;
    sta.arriving = train[0].arrivingTime[i];
    sta.leaving = train[0].leavingTime[i];
    Station.insert(train[0].stations[i].get_hashcode(), sta);
  }
  for (TIME d = train[0].sDate; d <= train[0].eDate; ++d) {
    DAYTRAIN day(train[0].seatNum);
    DayTrain.insert(std::make_pair(d, train[0].trainID.get_hashcode()), day);
  }
  return "0";
}

std::string query_train(CMD& cmd) {
  vector<TRAININFO> train = Trains.find(get_hashcode(cmd.cmd['i' - 'a']));
  if (train.empty()) return "-1";
  vector<DAYTRAIN> day;
  TIME date = cmd.cmd['d' - 'a'];
  if (train[0].pub) day = DayTrain.find(std::make_pair(date, train[0].trainID.get_hashcode()));
  std::string ret;
  ret += std::string(train[0].trainID.tid) + " " + train[0].type;
  for (int i = 0; i < train[0].stationNum; ++i) {
    ret += "\n ";
    ret += std::string(train[0].stations[i].sta) + " ";
    if (i == 0) ret += "xx-xx xx:xx";
    else ret += (date + train[0].arrivingTime[i]).string_date() + " " + (date + train[0].arrivingTime[i]).string_time();
    ret += " -> ";
    if (i == train[0].stationNum - 1) ret += "xx-xx xx:xx";
    else ret += (date + train[0].leavingTime[i]).string_date() + " " + (date + train[0].leavingTime[i]).string_time();
    ret += " " + std::to_string(train[0].cost[i]);
    if (train[0].pub) ret += " " + std::to_string(day[0].seat[i]);
    else ret += " " + std::to_string(train[0].seatNum);
    if (i == train[0].stationNum - 1) ret += " x";
    else ret += std::to_string(train[0].cost[i]);
  }
  return ret;
}

std::string query_ticket(CMD& cmd) {
  vector<STATION> fr = Station.find(get_hashcode(cmd.cmd['s' - 'a']));
  if (fr.empty()) return "-1";
  vector<STATION> to = Station.find(get_hashcode(cmd.cmd['t' - 'a']));
  if (to.empty()) return "-1";

  TIME day(cmd.cmd['d' - 'a']);
  vector<TICKET> tickets;
  for (int i = 0; i < fr.size(); ++i) {
    if (day < fr[i].sDate || fr[i].eDate < day) continue;
    for (int j = 0; j < to.size(); ++j) {
      if (day < to[j].sDate || to[j].eDate < day) continue;
      if (fr[i].trainID == to[j].trainID && fr[i].rank < to[j].rank) {
        TICKET new_ticket;
        new_ticket.trainID = fr[i].trainID;
        new_ticket.from = cmd.cmd['s' - 'a'];
        new_ticket.dest = cmd.cmd['t' - 'a'];
        new_ticket.leaving = day + fr[i].leaving;
        new_ticket.arriving = day + fr[i].arriving;
        new_ticket.cost = to[j].cost - fr[i].cost;
        vector<DAYTRAIN> dt = DayTrain.find(std::make_pair(day, fr[i].trainID.get_hashcode()));
        if (dt.empty()) new_ticket.seat = fr[i].seatNum;
        else new_ticket.seat = dt[0].query_seat(fr[i].rank, to[j].rank);
        tickets.push_back(new_ticket);
      }
    }
  }
  if (cmd.cmd['p' - 'a'] == "time") mergeSort(tickets, comp_time);
  else mergeSort(tickets, comp_cost);

  std::string ret;
  ret += std::to_string(tickets.size());
  for (int i = 0; i < tickets.size(); ++i) {
    ret += "\n" + tickets[i].show();
  }
  return ret;
}

#endif //TICKETSYSTEM_CORE_H
