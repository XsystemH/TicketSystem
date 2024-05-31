//
// Created by 13199 on 2024/5/26.
//

#ifndef TICKETSYSTEM_STORAGE_H
#define TICKETSYSTEM_STORAGE_H

#include "BPT.h"
#include "LinearStorage.h"
#include "map/map.hpp"
#include "Structs.h"

using hashcode = unsigned long long;

// user
// <uid.hash, privilege>
sjtu::map<hashcode, int> LoginStack;
// <uid.hash, USERINFO>
BBPT<hashcode, USERINFO> Users("USER_NODE", "USER_KEYS","USER_INFO");

// train
// <tid.hash, Raw>
BBPT<hashcode, TRAININFO> Trains("TRAIN_NODE", "TRAIN_KEYS", "TRAIN_INFO");
// <<DATE, tid.hash>, DayTrain(seat)>
BBPT<std::pair<TIME, hashcode>, DAYTRAIN> DayTrain("DAY_NODE", "DAY_KEYS", "DAY_INFO");
// <sta.hash, STATION>
BPT<hashcode, STATION> Station("STA_NODE", "STA_INFO");

// order
// <uid.hash, ORDER>
BBPT<hashcode, ORDER> Order("ORD_NODE", "ORD_KEYS", "ORD_INFO");
// <<tid.hash, date>, ORDER>
BPT<std::pair<hashcode, TIME>, ORDER> Pending("PEN_NODE", "PEN_INFO");

void clear_all() {
  LoginStack.clear();
  Users.clear();
  Trains.clear();
  DayTrain.clear();
  Station.clear();
}

#endif //TICKETSYSTEM_STORAGE_H
