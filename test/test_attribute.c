#include<time.h>
#include<stdlib.h>
#include "attribute.h"
int main(){
  srand(time(NULL));
  Attr a = rand_attr();
  print_attr(a);
  return 0;
}
