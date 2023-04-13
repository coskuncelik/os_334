
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <sys/wait.h>

int childFunction(int n);
int parentFunction(int no);


typedef struct {
    int m;
    int nums[100];
    float exec_time;
    int sig;
    char signal[64];
    char sig_rec[64];
} output_t ;


