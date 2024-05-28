#include <iostream>
#include <string>
#include "CommandProcesser.h"
#include "Structs.h"
#include "User.h"

int main() {
  CMD cmd;
  while (true) {
    cmd = get_cmd();
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