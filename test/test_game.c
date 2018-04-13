#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include "game.h"

int main(){
  srand(time(NULL));
  Map m = rand_map();
  run(m);
  destroy_map(m);
}
