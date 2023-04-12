
#include "part1.h"
#include "sort.h"


void main(int argc, char **argv)
{
    
    int file_count = atoi(argv[1]) ;

    pid_t pid[file_count] ;
    int i, child_status;

    for(i=0; i<file_count; i++)
    {
        if( (pid[i] = fork()) == 0  )
        {
            childFunction(i);
            exit(100+i);
        }
    }

    for(i=0; i<file_count; i++)
    {  

        pid_t wpid = wait(&child_status) ;
        if(WIFEXITED(child_status))
            printf("Child %d terminated with exit status %d \n", wpid, WEXITSTATUS(child_status));
        else
            printf("Child %d terminated abnormally\n", wpid);
    }
    parentFunction(file_count);
    
}


int childFunction(int no)
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
    for(i=0;i<100;i++)   // TODO: do not use 100. find another way for creating random nums..
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
    fprintf(OutputFile, "**** coming soon (for signal implementation) ****\n");


    // Close files
    fclose(InputFile);
    fclose(OutputFile);
    
    return 0;
}


int parentFunction(int file_count) {
    printf("parent function started\n");

    output_t outs[file_count];

    int i, j, n;
    char InputFileName[64], OutputFileName[64], FinalOutputFileName[64];
    int nums[100];
    float exec_time;

    for(i=0; i<file_count; i++)
    {
        sprintf(OutputFileName, "file/output%d.txt", i);
            
        FILE *OutputFile = fopen(OutputFileName, "r");
        if (OutputFile == NULL){
            printf("Cannot open output file %d \n", i);
            return 1;
        }

        fscanf(OutputFile,"%d\n", &outs[i].count);

        for(j=0; j<outs[i].count; j++)
            fscanf(OutputFile,"%d ", &outs[i].nums[j]);

        fscanf(OutputFile,"%f\n",&outs[i].exec_time);
    }
    
    SelectionSortForOuts(outs, file_count);

#if 0
    for(i=0; i<file_count; i++)
    {
        printf("%d\n",outs[i].count);
        for(j=0; j<outs[i].count; j++)
            printf("%d ", outs[i].nums[j]);
        printf("\n");
        printf("%f\n\n\n",outs[i].exec_time);
    }
#endif

    sprintf(FinalOutputFileName, "file/output.txt");
    FILE *FinalOutputFile = fopen(FinalOutputFileName, "w");
    if (FinalOutputFile == NULL){
        printf("Cannot open Final output file \n");
        return 1;
    }

    for(i=0; i<file_count; i++)
    {
        fprintf(FinalOutputFile, "%f - ", outs[i].exec_time);
        for(j=0; j<outs[i].count; j++)
            fprintf(FinalOutputFile, "%d ", outs[i].nums[j]);
        fprintf(FinalOutputFile, " - ");
        fprintf(FinalOutputFile, "SIG?? \n");
    }


    printf("parent function finished\n");
    return 0;
}


