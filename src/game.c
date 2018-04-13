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
#ifdef ARM
  lcd_write_info_t lcd;
  time_t start;
  if((fd = open(”/dev/lcd”, ORDWR)) < 0){
    printf(”Open /dev/lcd faild.\n”);
    exit(-1);
  }
  time( &start);
  init_lcd();
#endif
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
    print_map(m);
    printf("\n\n");
#endif
    sleep(0.5);
    if (end(m)){
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
