#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include "game.h"

#ifdef ARM
int fd;
#endif


void run(Map m){
  // prepare for the game start
  //bla bla blah
  time_t start;
#ifdef ARM
  lcd_write_info_t lcd;
  if((fd = open("/dev/lcd", O_RDWR)) < 0){
    printf("Open /dev/lcd faild.\n");
    exit(-1);
  }
  init_lcd();
#endif

  time( &start);
  while (1){
    //each car move
    for(int c=0;c<m.car_num;c++){
      //check car attribute overcome state
      int si = check_state(m, c);
      move_car(m, c, si);
    }
#ifdef ARM
    print_time(start, &lcd);
    draw_map(m, &lcd);
#else
    print_time(start);
    draw_map(m);
#endif
    usleep(200000);
    if (end(m)){
#ifndef ARM
      printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
#endif
      return ;
    }
  }
}

int end(Map m){
  int cnt =0;
  for (int c=0;c<m.car_num;c++){
    if (m.cars[c].phase == END || m.cars[c].phase == LOST)
      cnt++;
  }
  return cnt == m.car_num;
}
