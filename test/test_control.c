#include<stdlib.h>
#include<time.h>
#include "control.h"

int main(){
  srand(time(NULL));
  for (int i=0;i<10;i++){
    print_control(control(i));
  }
  return 0;
}
