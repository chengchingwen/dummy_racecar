#include<stdlib.h>
#include<time.h>
#include "car.h"

int main(){
  srand(time(NULL));
  Car c = rand_car();
  print_car(c);
  return 0;
}
