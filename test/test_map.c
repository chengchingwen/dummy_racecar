#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<map.h>

int main(){
  srand(time(NULL));
  Map m = rand_map();
  print_map(m);
  destroy_map(m);
  return 0;
}
