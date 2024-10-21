// header file where the patterns are defined! :DDDD

#ifndef FORKYTHREADY_H
#define FORKYTHREADY_H

typedef struct {
  int cnt;
} pattern_1_thread;

typedef struct {
  int cnt;
  int mx;
} pattern_2_thread;

unsigned int randomTime();

void *thread_pattern1(void *thr_arg);

void *thread_pattern2(void *thr_arg);

void run_patterns(int numOfThings, int pattern);

void pattern1(int numOfThings);

void pattern2(int numOfThings);

#endif