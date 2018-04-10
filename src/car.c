#include<stdio.h>
#include<stdlib.h>
#include "car.h"


Car init_car(int l,
             int b,
             int ms,
             int s,
             Car_phase p,
             Attr a1,
             Attr a2){
  Car c = {.location = l,
           .bias = b,
           .max_speed = ms,
           .speed = s,
           .phase = p,
           .attr1 = a1,
           .attr2 = a2
  };
  return c;
}

Car car(Attr a1, Attr a2){
  Car c = init_car(0, 0, 10, 0,
                   START_UP,
                   a1, a2
                   );
  return c;
}

Car rand_car(){
  return car(rand_attr(), rand_attr());
}

void print_phase(Car_phase p){
  printf("Car phase: ");
  switch (p){
  case START_UP:
    printf("STARTING\n");break;
  case RUNNING:
    printf("RUNNING\n");break;
  case LOST:
    printf("LOST\n");break;
  case END:
    printf("END\n");break;
  }
}

void print_car(Car c){
  printf("Location: %d\n"
         "Bias: %d\n"
         "Max speed: %d\n"
         "Current speed: %d\n"
         , c.location, c.bias, c.max_speed,
         c.speed);
  print_phase(c.phase);
  print_attr(c.attr1);
  print_attr(c.attr2);
}
