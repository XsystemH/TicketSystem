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

//  Users.erase(user[0].username.get_hashcode(), user[0]);

  if (!cmd.cmd['p' - 'a'].empty()) user[0].password = cmd.cmd['p' - 'a'];
  if (!cmd.cmd['n' - 'a'].empty()) user[0].name = cmd.cmd['n' - 'a'];
  if (!cmd.cmd['m' - 'a'].empty()) user[0].mailAddr = cmd.cmd['m' - 'a'];
  if (!cmd.cmd['g' - 'a'].empty()) user[0].privilege = to_num(cmd.cmd['g' - 'a']);

  Users.modify(user[0].username.get_hashcode(), user[0]);
  if (LoginStack.find(user[0].username.get_hashcode()) != LoginStack.end())
    LoginStack[user[0].username.get_hashcode()] = user[0].privilege;
  return user[0].information();
}

// Train

vector<std::string> divide_string(std::string s) {
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

vector<int> divide_num(std::string s) {
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
  vector<std::string> sName = divide_string(cmd.cmd['s' - 'a']);
  for (int i = 0; i < sName.size(); ++i) {
    new_train.stations[i] = sName[i];
  }
  vector<int> p = divide_num(cmd.cmd['p' - 'a']);
  for (int i = 0; i < p.size(); ++i) {
    new_train.prices[i] = p[i];
  }
  new_train.startTime = cmd.cmd['x' - 'a'];
  vector<int> tTime = divide_num(cmd.cmd['t' - 'a']);
  for (int i = 0; i < tTime.size(); ++i) {
    new_train.travelTimes[i] = tTime[i];
  }
  vector<int> sTime = divide_num(cmd.cmd['o' - 'a']);
  for (int i = 0; i < sTime.size(); ++i) {
    new_train.stopoverTimes[i + 1] = sTime[i];
  }
  vector<std::string> sDate = divide_string(cmd.cmd['d' - 'a']);
  new_train.saleDate[0] = sDate[0];
  new_train.saleDate[1] = sDate[1];
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
  if (train[0].pub) return "-1";
//  Trains.erase(train[0].trainID.get_hashcode(), train[0]);
  train[0].pub = true;
  Trains.modify(train[0].trainID.get_hashcode(), train[0]);
  // pub
  for (int i = 0; i < train[0].stationNum; ++i) {
    STATION new_station(train[0], i);
    Station.insert(train[0].stations[i].get_hashcode(), new_station);
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
  if (train[0].pub) {
    day = DayTrain.find(std::make_pair(date, train[0].trainID.get_hashcode()));
    if (day.empty()) return "-1";
  }
  else {
    if (date < train[0].sDate || train[0].eDate < date)
      return "-1";
  }
  std::string ret;
  ret += std::string(train[0].trainID.tid) + " " + train[0].type;
  for (int i = 0; i < train[0].stationNum; ++i) {
    ret += "\n";
    ret += std::string(train[0].stations[i].sta) + " ";
    if (i == 0) ret += "xx-xx xx:xx";
    else ret += (date + train[0].arrivingTime[i]).string_date() + " " + (date + train[0].arrivingTime[i]).string_time();
    ret += " -> ";
    if (i == train[0].stationNum - 1) ret += "xx-xx xx:xx";
    else ret += (date + train[0].leavingTime[i]).string_date() + " " + (date + train[0].leavingTime[i]).string_time();
    ret += " " + std::to_string(train[0].cost[i]);
    if (i == train[0].stationNum - 1)  ret += " x";
    else {
      if (train[0].pub) ret += " " + std::to_string(day[0].seat[i]);
      else ret += " " + std::to_string(train[0].seatNum);
    }
  }
  return ret;
}

TICKET get_ticket(const std::string s1, const std::string s2, const STATION& fr, const STATION& to, const TIME day) {
  TICKET new_ticket;
  new_ticket.trainID = fr.trainID;
  new_ticket.from = s1;
  new_ticket.dest = s2;
  new_ticket.leaving = day + fr.leaving.get_time();
  new_ticket.arriving = new_ticket.leaving - fr.leaving + to.arriving;
  new_ticket.cost = to.cost - fr.cost;
  vector<DAYTRAIN> dt = DayTrain.find(std::make_pair(day - fr.leaving.get_date(), fr.trainID.get_hashcode()));
  new_ticket.seat = dt[0].query_seat(fr.rank, to.rank);
  return new_ticket;
}

vector<TICKET> query_ticket(std::string s, std::string t, TIME d) {
  vector<TICKET> ans;
  if (get_hashcode(s) == get_hashcode(t))
    return ans;
  vector<STATION> fr = Station.find(get_hashcode(s));
  if (fr.empty()) return ans;
  vector<STATION> to = Station.find(get_hashcode(t));
  if (to.empty()) return ans;

  vector<TICKET> tickets;
  for (int i = 0; i < fr.size(); ++i) {
    if (fr[i].eDate + fr[i].leaving < d) continue;
    for (int j = 0; j < to.size(); ++j) {
      if (fr[i].trainID == to[j].trainID && fr[i].rank < to[j].rank) {
        TIME day(d);
        if (d < fr[i].sDate + fr[i].leaving) tickets.push_back(get_ticket(s, t, fr[i], to[j], (fr[i].sDate + fr[i].leaving).get_date()));
        else if (fr[i].leaving.get_time() < d.get_time()) {
          ++day;
          if (day < fr[i].eDate + fr[i].leaving) tickets.push_back(get_ticket(s, t, fr[i], to[j], day.get_date()));
        }
        else tickets.push_back(get_ticket(s, t, fr[i], to[j], d.get_date()));
      }
    }
  }
  if (tickets.empty()) {
    return ans;
  }
  mergeSort(tickets, comp_cost);

  return tickets;
}

std::string query_ticket(CMD& cmd) {
  if (get_hashcode(cmd.cmd['s' - 'a']) == get_hashcode(cmd.cmd['t' - 'a']))
    return "0";
  vector<STATION> fr = Station.find(get_hashcode(cmd.cmd['s' - 'a']));
  if (fr.empty()) return "0";
  vector<STATION> to = Station.find(get_hashcode(cmd.cmd['t' - 'a']));
  if (to.empty()) return "0";

  TIME day(cmd.cmd['d' - 'a']); // 站点日期(非始发站)
  vector<TICKET> tickets;
  for (int i = 0; i < fr.size(); ++i) {
    if (day < (fr[i].sDate + fr[i].leaving).get_date() || (fr[i].eDate + fr[i].leaving).get_date() < day) continue;
    for (int j = 0; j < to.size(); ++j) {
//      if (day < to[j].sDate || to[j].eDate < day) continue; 到达时间不可控 仅需判断到达是否为同车次
      if (fr[i].trainID == to[j].trainID && fr[i].rank < to[j].rank) {
        tickets.push_back(get_ticket(cmd.cmd['s' - 'a'], cmd.cmd['t' - 'a'], fr[i], to[j], day));
      }
    }
  }
  if (tickets.empty()) {
    return "0";
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

std::string query_transfer(CMD& cmd) {
  if (get_hashcode(cmd.cmd['s' - 'a']) == get_hashcode(cmd.cmd['t' - 'a']))
    return "0";
  vector<STATION> fr = Station.find(get_hashcode(cmd.cmd['s' - 'a']));
  if (fr.empty()) return "0";
  vector<STATION> to = Station.find(get_hashcode(cmd.cmd['t' - 'a']));
  if (to.empty()) return "0";

  TIME day(cmd.cmd['d' - 'a']);
  vector<std::pair<TICKET, TICKET>> ticket;
  for (int i = 0; i < fr.size(); ++i) {
    if (day < (fr[i].sDate + fr[i].leaving).get_date() || (fr[i].eDate + fr[i].leaving).get_date() < day) continue;
    vector<TRAININFO> train = Trains.find(fr[i].trainID.get_hashcode());
    for (int j = fr[i].rank + 1; j < train[0].stationNum; ++j) {
      STATION mid =  STATION(train[0], j);
      TICKET fir = get_ticket(cmd.cmd['s' - 'a'], std::string(train[0].stations[j].sta), fr[i], mid, day);
      vector<TICKET> sec = query_ticket(std::string(train[0].stations[j].sta), cmd.cmd['t' - 'a'], fir.arriving);
      for (int k = 0; k < sec.size(); ++k) {
        if (fir.trainID != sec[k].trainID) ticket.push_back(std::make_pair(fir, sec[k]));
      }
    }

  }
  if (ticket.empty()) return "0";
  if (cmd.cmd['p' - 'a'] == "time") mergeSort(ticket, comp_time_sum);
  else mergeSort(ticket, comp_cost_sum);

  return ticket[0].first.show() + "\n" + ticket[0].second.show();
}

// order

std::string buy_ticket(CMD& cmd) {
  if (LoginStack.find(get_hashcode(cmd.cmd['u' - 'a'])) == LoginStack.end())
    return "-1";
  TIME date = cmd.cmd['d' - 'a'];
  vector<TRAININFO> train = Trains.find(get_hashcode(cmd.cmd['i' - 'a']));
  if (train.empty()) return "-1";
  if (!train[0].pub) return "-1";
  hashcode fr_hash =  get_hashcode(cmd.cmd['f' - 'a']);
  hashcode to_hash =  get_hashcode(cmd.cmd['t' - 'a']);
  if (fr_hash == to_hash) return "-1";
  STATION fr, to;
  for (int i = 0; i < train[0].stationNum; ++i) {
    if (train[0].stations[i].get_hashcode() == fr_hash) {
      fr = STATION(train[0], i);
    }
    if (train[0].stations[i].get_hashcode() == to_hash) {
      to = STATION(train[0], i);
      break;
    }
  }
  if (fr.rank >= to.rank) return "-1";
  if (date < (fr.sDate + fr.leaving.get_date()) || (fr.eDate + fr.leaving.get_date()) < date)
    return "-1";
  vector<DAYTRAIN> day = DayTrain.find(std::make_pair(date - fr.leaving.get_date(), train[0].trainID.get_hashcode()));
  if (day.empty()) return "-1";
  int need = to_num(cmd.cmd['n' - 'a']);
  ORDER new_order;
  new_order.rank = Order.size();
  new_order.userID = cmd.cmd['u' - 'a'];
  new_order.trainID = fr.trainID;
  new_order.date = date - fr.leaving.get_date(); // 出发日期
  new_order.from = train[0].stations[fr.rank];
  new_order.fr_rank = fr.rank;
  new_order.dest = train[0].stations[to.rank];
  new_order.to_rank = to.rank;
  new_order.leaving = date + fr.leaving.get_time();
  new_order.arriving = new_order.leaving - fr.leaving + to.arriving;
  new_order.cost = train[0].cost[to.rank] - train[0].cost[fr.rank];
  new_order.seat = need;
  if (need <= day[0].query_seat(fr.rank, to.rank)) {
    day[0].modify_seat(fr.rank, to.rank, -need);
    DayTrain.modify(std::make_pair(date - fr.leaving.get_date(), fr.trainID.get_hashcode()), day[0]);
    new_order.status = 0;
    Order.insert(new_order.userID.get_hashcode(), new_order);
    return std::to_string(new_order.cost * need);
  }
  else {
    if (cmd.cmd['q' - 'a'] == "true" && need <= day[0].num) {
      new_order.status = 1;
      Order.insert(new_order.userID.get_hashcode(), new_order);
      Pending.insert(std::make_pair(new_order.trainID.get_hashcode(), new_order.date), new_order);
      return "queue";
    }
    else {
      return "-1";
    }
  }
}

std::string query_order(CMD& cmd) {
  if (LoginStack.find(get_hashcode(cmd.cmd['u' - 'a'])) == LoginStack.end()) return "-1";
  std::string ret;
  vector<ORDER> orders = Order.find(get_hashcode(cmd.cmd['u' - 'a']));
  ret += std::to_string(orders.size());
  if (orders.empty()) return ret;
  for (int i = orders.size() - 1; i >= 0; --i) {
    ret += "\n" + orders[i].show();
  }
  return ret;
}

std::string refund_ticket(CMD& cmd) {
  if (LoginStack.find(get_hashcode(cmd.cmd['u' - 'a'])) == LoginStack.end()) return "-1";
  vector<ORDER> orders = Order.find(get_hashcode(cmd.cmd['u' - 'a']));
  if (orders.empty()) return "-1";
  int n = 0;
  if (!cmd.cmd['n'- 'a'].empty()) n = orders.size() - to_num(cmd.cmd['n'- 'a']);
  if (n < 0) return "-1";

  if (orders[n].status) { // not success
    if (orders[n].status == 1) Pending.erase(std::make_pair(orders[n].trainID.get_hashcode(), orders[n].date), orders[n]);
    else return "-1";
    orders[n].status = 2;
    Order.modify(orders[n].userID.get_hashcode(), orders[n], n);
    return "0";
  }
  vector<DAYTRAIN> dt = DayTrain.find(std::make_pair(orders[n].date, orders[n].trainID.get_hashcode()));
  vector<ORDER> pending = Pending.find(std::make_pair(orders[n].trainID.get_hashcode(), orders[n].date));
  dt[0].modify_seat(orders[n].fr_rank, orders[n].to_rank, orders[n].seat);
  for (int i = 0; i < pending.size(); ++i) {
    if (pending[i].seat <= dt[0].query_seat(pending[i].fr_rank, pending[i].to_rank)) {
      dt[0].modify_seat(pending[i].fr_rank, pending[i].to_rank, -pending[i].seat);
      Pending.erase(std::make_pair(pending[i].trainID.get_hashcode(), pending[i].date), pending[i]);
      pending[i].status = 0;
      Order.modify_(pending[i].userID.get_hashcode(), pending[i]);
    }
  }
  DayTrain.modify(std::make_pair(orders[n].date, orders[n].trainID.get_hashcode()), dt[0]);
  orders[n].status = 2;
  Order.modify(orders[n].userID.get_hashcode(), orders[n], n);
  return "0";
}
#endif //TICKETSYSTEM_CORE_H
