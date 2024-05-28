//
// Created by 13199 on 2024/5/28.
//

#ifndef TICKETSYSTEM_USER_H
#define TICKETSYSTEM_USER_H

#include "vector/vector.hpp"
#include "Storage.h"

int to_num(std::string s) {
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

  hashcode cur = UIDTYPE(cmd.cmd['u' - 'a']).get_hashcode();
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
  hashcode cur = UIDTYPE(cmd.cmd['u' - 'a']).get_hashcode();
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
  hashcode cur = UIDTYPE(cmd.cmd['u' - 'a']).get_hashcode();
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

#endif //TICKETSYSTEM_USER_H
