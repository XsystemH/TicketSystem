//
// Created by 13199 on 2024/5/26.
//

#ifndef TICKETSYSTEM_COMMANDPROCESSER_H
#define TICKETSYSTEM_COMMANDPROCESSER_H

#include "Structs.h"
#include <iostream>
#include <string>

CMD process(const std::string &s) {
  CMD cmd;
  std::istringstream input(s);
  std::string CmdType, CmdValue;
  input >> cmd.timestamp >> cmd.type;
  while (input >> CmdType >> CmdValue) {
    cmd.num++;
    cmd.cmd[CmdType[1] - 'a'] = CmdValue;
  }
  return cmd;
}

CMD get_cmd() {
  std::string s;
  std::getline(std::cin, s);
  return process(s);
}

#endif //TICKETSYSTEM_COMMANDPROCESSER_H
