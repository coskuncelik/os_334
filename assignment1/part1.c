
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <sys/wait.h>
#include "sort.h"

int function(int n);

#if 1
#define N 5 

void main(int argc, char **argv)
{
    pid_t pid[N] ;
    int i, child_status;

    for(i=0; i<N; i++)
    {
        if( (pid[i] = fork()) == 0  )
        {
            function(i);
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
#endif

int function(int no)
{
    // Definitions
    char InputFileName[64], OutputFileName[64];
    int nums[100];
    int n, i;

    // Get start time
    struct timeval  tv1, tv2;
    gettimeofday(&tv1, NULL);

    // Create file names
    sprintf(InputFileName,  "file/input%d.txt",  no);
    sprintf(OutputFileName, "file/output%d.txt", no);


    // Open files
    FILE *InputFile = fopen(InputFileName, "r");
    if (InputFile == NULL){
        printf("Cannot open input file %d \n", no);
        return 1;
    }

    FILE *OutputFile = fopen(OutputFileName, "w");
    if (OutputFile == NULL){
        printf("Cannot open output file %d \n", no);
        return 1;
    }

    // Read input file
    fscanf(InputFile,"%d\n",&n);

    for(i=0; i<n; i++)
        fscanf(InputFile,"%d ", &nums[i]);

    // Sort numbers with appropriate sorting function
    if( (n%2) == 1)
        SelectionSort(nums, n);
    else
        InsertionSort(nums, n);

#if 0   
    for(i=0; i<n; i++)
        printf("%d ", nums[i]);
    printf("\n");
#endif
    
    // Random sleep between 1-7 seconds
    int stime;
    srand(time(0));
    for(i=0;i<N;i++)
    {
        stime = (rand() % 7) + 1 ;
        if(i==no)  
            break;
    }   
    sleep(stime);

    // Get end time & calculate execution time
    gettimeofday(&tv2, NULL);
    double time_spent = (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec) ;
#if 1
    printf ("Total time = %f seconds\n", time_spent);
#endif

    //  Writes the results to the intermediate output file
    fprintf(OutputFile, "%d\n", n);
    for(i=0; i<n; i++)
        fprintf(OutputFile, "%d ", nums[i]);
    fprintf(OutputFile, "\n");
    fprintf(OutputFile, "%f \n", time_spent);
    fprintf(OutputFile, "**** coming soon ****\n");


    // Close files
    fclose(InputFile);
    fclose(OutputFile);
    
    return 0;
}

