#include <iostream>
#include <cstdio>
#include <string>
#include "CommandProcesser.h"
#include "Structs.h"
#include "Core.h"

int main() {
//  freopen("in", "r", stdin);
//  freopen("out", "w", stdout);
  std::ios::sync_with_stdio(0),std::cin.tie(0),std::cout.tie(0);
//取消同步流

  CMD cmd;
  while (true) {
    cmd = get_cmd();
    std::cout << cmd.timestamp << " ";
    if (cmd.type == "add_user") {
      std::cout << add_user(cmd) << std::endl;
      continue;
    }
    if (cmd.type == "login") {
      std::cout << login(cmd) << std::endl;
      continue;
    }
    if (cmd.type == "logout") {
      std::cout << logout(cmd) << std::endl;
      continue;
    }
    if (cmd.type == "query_profile") {
      std::cout << query_profile(cmd) << std::endl;
      continue;
    }
    if (cmd.type == "modify_profile") {
      std::cout << modify_profile(cmd) << std::endl;
      continue;
    }
    if (cmd.type == "add_train") {
      std::cout << add_train(cmd) << std::endl;
      continue;
    }
    if (cmd.type == "delete_train") {
      std::cout << delete_train(cmd) << std::endl;
      continue;
    }
    if (cmd.type == "release_train") {
      std::cout << release_train(cmd) << std::endl;
      continue;
    }
    if (cmd.type == "query_train") {
      std::cout << query_train(cmd) << std::endl;
      continue;
    }
    if (cmd.type == "query_ticket") {
      std::cout << query_ticket(cmd) << std::endl;
      continue;
    }
    if (cmd.type == "query_transfer") {
      std::cout << query_transfer(cmd) << std::endl;
      continue;
    }
    if (cmd.type == "buy_ticket") {
      std::cout << buy_ticket(cmd) << std::endl;
      continue;
    }
    if (cmd.type == "query_order") {
      std::cout << query_order(cmd) << std::endl;
      continue;
    }
    if (cmd.type == "refund_ticket") {
      std::cout << refund_ticket(cmd) << std::endl;
      continue;
    }
    if (cmd.type == "clear") {
      std::cout << "0" << std::endl;
      clear_all();
      continue;
    }
    if (cmd.type == "exit") {
      std::cout << "bye" << std::endl;
      break;
    }
  }
  return 0;
}