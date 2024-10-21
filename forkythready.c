// implementation file where the patterns are implemented

#include "forkythready.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

unsigned int randomTime() { return (rand() % 8) + 1; }

void *thread_pattern1(void *thr_arg) {
    pattern_1_thread *target = (pattern_1_thread *)thr_arg;
    printf("Thread %d created with TID: %lu\n", target->cnt, pthread_self());
    sleep(randomTime());
    // free the allocated memowyyyy
    free(target);
    return NULL;
}

void *thread_pattern2(void *thr_arg) {
    pattern_2_thread *target = (pattern_2_thread *)thr_arg;
    // assign to keep track of Thread ID for parent
    long long unsigned parentTID = pthread_self();
    printf("Thread %d created with TID: %llu\n", target->cnt, parentTID);
    sleep(randomTime());

    if (target->cnt < target->mx) {
        pthread_t exThread;
        // copy properties since we need to create new thread
        pattern_2_thread *newTarget = malloc(sizeof(pattern_2_thread));
        newTarget->cnt = target->cnt + 1;
        newTarget->mx = target->mx;

        printf("Thread %d with TID: %llu is now creating Thread: %d\n",
               target->cnt, parentTID, newTarget->cnt);

        // create new thread with pthread library
        pthread_create(&exThread, NULL, thread_pattern2, newTarget);
        pthread_join(exThread, NULL);

        // fwee da memowyy
        free(newTarget);
    }
    printf("Thread %d has exited with TID: %lu\n", target->cnt, pthread_self());

    return NULL;
}

void run_patterns(int numOfThings, int pattern) {
    switch (pattern) {
        case 1:
            printf("Running pattern 1\n");
            pattern1(numOfThings);
            break;
        case 2:
            printf("Running pattern 2\n");
            pattern2(numOfThings);
            break;
    }
}

// forking multiple processes from the main (fork ALL processes before waiting
// for any of them)
void pattern1(int numOfThings) {
    // printf("Test print\n");
    printf("Pattern 1 main before creating any processes\n");
    pthread_t exThreads[numOfThings];

    for (int ix = 0; ix < numOfThings; ix++) {
        pattern_1_thread *targ = malloc(sizeof(pattern_1_thread));
        targ->cnt = ix + 1;
        pthread_create(&exThreads[ix], NULL, thread_pattern2, targ);
    }
    for (int ix = 0; ix < numOfThings; ix++) {
        pthread_join(exThreads[ix], NULL);
        printf("Thread %d exiting with PID: %ld\n", ix + 1, pthread_self());
    }
    printf("Main of pattern 1 is now exiting.\n");
}

// forking a single process from the main and then passing it on to the next
// process
void pattern2(int numOfThings) {
    // printf("Test print\n");
    printf("Pattern 2 main before creating any processes\n");
    pthread_t exThread;

    pattern_2_thread *targ = malloc(sizeof(pattern_2_thread));
    targ->cnt = 1;
    targ->mx = numOfThings;
    pthread_create(&exThread, NULL, thread_pattern2, targ);
    pthread_join(exThread, NULL);
    free(targ);
    printf("Main of pattern 2 is now exiting.\n");
}