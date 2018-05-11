#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include "display.h"
//#include "map.h"
int fd;


int main(){
  Map m = map(100, 1, 1);
  DPBUFFER(dpbuffer);
  lcd_write_info_t lcd;
  if((fd = open("/dev/lcd", O_RDWR)) < 0){
    printf("Open /dev/lcd faild.\n");
    exit(-1);
  }
  printf("fd open success\n");
  init_lcd();
  time_t t;
  time(&t);
  m.states = malloc(sizeof(State));
  m.cars = malloc(sizeof(Car));
  m.states[0] = state(10, 0, 1, 30, POOL);
  m.cars[0] = rand_car();
  print_map(m);
  printf("\n");
  getframe(m, 0, dpbuffer);
#ifdef ARM
  sevseg(13);
  spark_led();
  usleep(200);
  spark_led();
  print_time(t , &lcd);
  draw_map(dpbuffer, &lcd);
#else
  draw_map(dpbuffer);
#endif
  printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
  destroy_map(m);
  return 0;
}
