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
sjtu::map<hashcode, int> LoginStack;
BPT<hashcode, USERINFO> Users("USER_NODE", "USER_INFO");

// train
BPT<hashcode, TRAININFO_RAW> Trains_Raw("TRAIN_NODE", "TRAIN_INFO");
BPT<std::pair<DATE, hashcode>, DAYTRAIN> DayTrain("DAY_NODE", "DAY_INFO");
BPT<hashcode, TRAININFO> Pub_Train("PUB_NODE", "PUB_INFO");
BPT<std::pair<hashcode, DATE>, STATYPE> Station("STA_NODE", "STA_INFO");

void clear_all() {
  LoginStack.clear();
  Users.clear();
  Trains_Raw.clear();
  DayTrain.clear();
  Pub_Train.clear();
  Station.clear();
}

#endif //TICKETSYSTEM_STORAGE_H
