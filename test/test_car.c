#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include "car.h"

int main(){
  srand(time(NULL));
  Car c = rand_state_car();
  print_car(c);
  for (int i=0;i<5;i++){
    printf("\n");
    CONTROL ctrl = control(0);
    print_control(ctrl);
    update_car(&c, ctrl);
    print_car(c);
  }
  return 0;
}
