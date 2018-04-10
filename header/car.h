#pragma once
#include "attribute.h"

typedef enum{
  START_UP,
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
Car rand_car();
void print_phase(Car_phase p);
void print_car(Car c);
