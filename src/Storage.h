//
// Created by 13199 on 2024/5/26.
//

#ifndef TICKETSYSTEM_STORAGE_H
#define TICKETSYSTEM_STORAGE_H

#include "BPT.h"
#include "map/map.hpp"
#include "Structs.h"

using hashcode = unsigned long long;

// user

// <uid.hash, privilege>
sjtu::map<hashcode, int> LoginStack;

// <uid.hash, USERINFO>
BPT<hashcode, USERINFO> Users("USER_NODE", "USER_INFO");

// train

// <tid.hash, Raw>
BPT<hashcode, TRAININFO> Trains("TRAIN_NODE", "TRAIN_INFO");

// <<DATE, tid.hash>, DayTrain(seat)>
BPT<std::pair<TIME, hashcode>, DAYTRAIN> DayTrain("DAY_NODE", "DAY_INFO");

// <sta.hash, STATION>
BPT<hashcode, STATION> Station("STA_NODE", "STA_INFO");

void clear_all() {
  LoginStack.clear();
  Users.clear();
  Trains.clear();
  DayTrain.clear();
  Station.clear();
}

#endif //TICKETSYSTEM_STORAGE_H
