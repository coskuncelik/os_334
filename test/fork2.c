#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define N 5 

void main()
{
    pid_t pid[N] ;
    int i, child_status;

    for(i=0; i<N; i++)
    {
        if( (pid[i] = fork()) == 0  )
        {
            // Random sleep between 1-7 seconds
            srand(time(NULL));
            int stime = (rand() % 7) + 1 + i;
            sleep(stime);
            printf("Printf from %d sleep: %d\n", getpid() , stime );
            exit(100+i);
        }
    }

    for(i=0; i<N; i++)
    {
        pid_t wpid = wait(&child_status) ;
        if(WIFEXITED(child_status))
            printf("Child %d terminated with exit status %d \n", wpid, WEXITSTATUS(child_status));
        else
            printf("Child %d terminated abnormally\n", wpid);
    }
    
}