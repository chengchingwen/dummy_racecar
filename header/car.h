#pragma once
#include "param.h"
#include "attribute.h"
#include "control.h"



typedef enum{
  RUNNING,
  LOST,
  END
}Car_phase;

typedef struct car{
  int location;
  int bias;
  int max_speed;
  int speed;
  Car_phase phase;
  Attr attr1;
  Attr attr2;
}Car;

Car init_car(int l,
             int b,
             int ms,
             int s,
             Car_phase p,
             Attr a1,
             Attr a2);

Car car(Attr a1, Attr a2);
Car rand_state_car();
Car rand_car();
void print_phase(Car_phase p);
void print_car(Car c);
int has_attr(Car c, Attr_type at);
void update_car(Car * cp, CONTROL ctrl);
