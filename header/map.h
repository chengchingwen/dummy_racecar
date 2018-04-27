#pragma once
#include "param.h"
#include "state.h"
#include "car.h"
#include "control.h"

typedef struct map{
  int length;
  int state_num, car_num;
  State* states;
  Car* cars;
}Map;

Map map(int l, int sn, int cn);
void destroy_map(Map m);
void print_map(Map m);
Map empty_map(int max_car);
Map rand_map();

int cover(State s, Car c);
int check_state(Map m, int i);
int move_car(Map m, int c, int si, CONTROL ctrl);
