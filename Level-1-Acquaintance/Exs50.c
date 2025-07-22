#include <stdio.h>
#include <time.h>

typedef struct timespec timespec;
struct timespec {
  time_t tv_sec; // Whole seconds >= 0
  long tv_nsec; // Nanoseconds [0, 999999999]            
};

timespec timespec_diff(timespec beginning, timespec end) {
  timespec res;
  
  if (end.tv_nsec >= beginning.tv_nsec) {
    res.tv_sec = end.tv_sec - beginning.tv_sec;
    res.tv_nsec = end.tv_nsec - beginning.tv_nsec;
  } else {
    res.tv_sec = end.tv_sec - beginning.tv_sec - 1;
    res.tv_nsec = 1000000000L + end.tv_nsec - beginning.tv_nsec;
  }
  return res;
}

int main(int argc, char* argv[argc+1]) {
  struct timespec start_time, end_time, duration;

  start_time.tv_sec = 100;
  start_time.tv_nsec = 500000000;
  end_time.tv_sec = 105;
  end_time.tv_nsec = 200000000;
  duration = timespec_diff(start_time, end_time);
  printf("Start: %ld s, %ld ns\n", start_time.tv_sec, start_time.tv_nsec);
  printf("End:   %ld s, %ld ns\n", end_time.tv_sec, end_time.tv_nsec);
  printf("Diff:  %ld s, %ld ns\n", duration.tv_sec, duration.tv_nsec);

  start_time.tv_sec = 200;
  start_time.tv_nsec = 500000000;
  end_time.tv_sec = 205;
  end_time.tv_nsec = 700000000;
  duration = timespec_diff(start_time, end_time); 
  printf("Start: %ld s, %ld ns\n", start_time.tv_sec, start_time.tv_nsec);
  printf("End:   %ld s, %ld ns\n", end_time.tv_sec, end_time.tv_nsec);
  printf("Diff:  %ld s, %ld ns\n", duration.tv_sec, duration.tv_nsec);

  start_time.tv_sec = 100;
  start_time.tv_nsec = 50000;
  end_time.tv_sec = 100;
  end_time.tv_nsec = 800000;
  duration = timespec_diff(start_time, end_time);
  printf("Start: %ld s, %ld ns\n", start_time.tv_sec, start_time.tv_nsec);
  printf("End:   %ld s, %ld ns\n", end_time.tv_sec, end_time.tv_nsec);
  printf("Diff:  %ld s, %ld ns\n", duration.tv_sec, duration.tv_nsec);

  return 0;
}
