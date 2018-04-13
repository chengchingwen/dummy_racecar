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
                   RUNNING,
                   a1, a2
                   );
  return c;
}

Car rand_car(){
  return car(rand_attr(), rand_attr());
}

Car rand_state_car(){
  Car c = init_car(rand()%100, rand() % MAXMAPWIDTH, rand()%10 + 10, rand()%10, rand() % 3, rand_attr(), rand_attr());
  return c;
}

void print_phase(Car_phase p){
  printf("Car phase: ");
  switch (p){
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

int has_attr(Car c, Attr_type at){
  return c.attr1.type == at || c.attr2.type == at;
}

void update_car(Car * cp, CONTROL ctrl){
  int delta_x = 0;
  int delta_y = 0;
  switch(ctrl){
  case FORWARD:
    delta_x = cp->max_speed / 5;
    break;
  case BACKWARD:
    delta_x = -(cp->max_speed / 10);
    break;
  case RIGHT:
    delta_y = +2;
    break;
  case LEFT:
    delta_y = -2;
    break;
  default:
    break;
  }
  cp->speed += delta_x;
  cp->bias += delta_y;
  if (cp->speed > cp->max_speed){
    cp->speed = cp->max_speed;
  }
  else if (cp->speed < 0){
    cp->speed = 0;
  }
  if (abs(cp->bias) > MAXMAPWIDTH){
    cp->bias /= cp->bias;
    cp->bias *= MAXMAPWIDTH;
  }
}
