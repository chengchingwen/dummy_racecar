#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<string.h>
#include "game.h"

#include "sh.h"

#ifdef ARM
int fd;
#endif
extern int END_GAME;

void run(Map m){
  // prepare for the game start
  time_t start;

  //boom handler
  struct sigaction sa;
  memset(&sa, 0, sizeof(sa));
  sa.sa_handler = &boomed;
  sigaction(SIGVTALRM, &sa, NULL);
  init_queue(&m);
  //end handler
  signal(SIGUSR1, end_game);
  
  //prepare for arm board lcd
  DPBUFFER(dpbuffer);
#ifdef ARM
  lcd_write_info_t lcd;
  if((fd = open("/dev/lcd", O_RDWR)) < 0){
    printf("Open /dev/lcd faild.\n");
    exit(-1);
  }
  init_lcd();
  ioctl(fd, KEY_IOCTL_CLEAR, NULL);
#endif
  //start time
  time(&start);

  //show # of cars
  sevseg(m.car_num);
  //game loop
  while (1){
    //each car move
    for(int c=0;c<m.car_num;c++){
      int si = check_state(m, c);
      if (c == 0 && si != -1)
        spark_led();
      else
        turn_off_led();

      CONTROL ctrl = control(c);
      move_car(m, c, si, ctrl);
    }
    //get current map frame
    getframe(m, 0, dpbuffer);

    //display frame
#ifdef ARM
    print_time(start, &lcd);
    draw_map(dpbuffer, &lcd);
#else
    print_time(start);
    draw_map(dpbuffer);
#endif

    usleep(200000);
    sb_win(m);
    if (end(m) || END_GAME){
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

void sb_win(Map m){
  for (int c=0;c<m.car_num;c++){
    if (m.cars[c].phase == END){
      kill(getpid(), SIGUSR1);
    }
  }
}
