
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <string.h>

int childFunction(int n, int rand17);
int parentFunction(int no);


typedef struct {
    int m;
    int nums[100];
    float exec_time;
    int sig;
    char signal[64];
    char sig_rec[64];
} output_t ;


