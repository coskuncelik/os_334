#include "part1.h"
#include "sort.h"

// Declarations
void signal_handler(int sig);
int sig_received = 999;
struct timeval sig_rec_time;

void main(int argc, char **argv){

    // Declarations
    int n = atoi(argv[1]); // file count
    pid_t pid[n] ;
    int i, child_status;

    // Random number seed
    srand(time(0));

    // Set user defined signals
    signal(SIGUSR1, signal_handler);
    signal(SIGUSR2, signal_handler);

    for(i=0; i<n; i++){
        if( (pid[i] = fork()) == 0  ){
            childFunction(i, rand()%7+1);
            exit(100+i);
        }
    }

    for(i=0; i<n; i++){
        // determine active child
        if(  (waitpid(pid[i], &child_status, WNOHANG )) == 0 ){
            if(pid[i]%2 == 0)
                kill(pid[i], SIGUSR2);
            else
                kill(pid[i], SIGUSR1);
        }
    }

    for(i=0; i<n; i++){  
        pid_t wpid = wait(&child_status) ;
        if(WIFEXITED(child_status))
            printf("Child %d terminated with exit status %d \n", wpid, WEXITSTATUS(child_status));
        else
            printf("Child %d terminated abnormally\n", wpid);
    }
    parentFunction(n);
    
}

void signal_handler(int sig) {
    sig_received = sig;
    gettimeofday(&sig_rec_time, NULL);
}

int childFunction(int no, int rand17) {

    // Declarations
    char InputFileName[64], OutputFileName[64];
    int nums[100];
    int m, i;

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
    fscanf(InputFile,"%d\n",&m);

    for(i=0; i<m; i++)
        fscanf(InputFile,"%d ", &nums[i]);

    // Sort numbers with appropriate sorting function
    if( (getpid()%2) == 1)
        SelectionSort(nums, m);
    else
        InsertionSort(nums, m);
   
    // Random sleep between 1-7 seconds
    sleep(rand17);

    // Get end time & calculate execution time
    gettimeofday(&tv2, NULL);
    double time_spent = (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec) ;

#if 0
    printf ("Total time = %f seconds\n", time_spent);
#endif

    //  Writes the results to the intermediate output file
    fprintf(OutputFile, "%d\n", m);
    for(i=0; i<m; i++)
        fprintf(OutputFile, "%d ", nums[i]);
    fprintf(OutputFile, "\n");
    fprintf(OutputFile, "%f \n", time_spent);
    fprintf(OutputFile, "%s %f\n", ( (sig_received==10)?("SIGUSR1"):("SIGUSR2") ) , sig_rec_time.tv_sec + (double)sig_rec_time.tv_usec/1000000 );

    // Close files
    fclose(InputFile);
    fclose(OutputFile);
    
    return 0;
}


int parentFunction(int n) {

    output_t outs[n];

    int i, j ;
    char InputFileName[64], OutputFileName[64], FinalOutputFileName[64];
    int nums[100];
    float exec_time;

    for(i=0; i<n; i++){

        sprintf(OutputFileName, "file/output%d.txt", i);
            
        FILE *OutputFile = fopen(OutputFileName, "r");
        if (OutputFile == NULL){
            printf("Cannot open output file %d \n", i);
            return 1;
        }

        fscanf(OutputFile,"%d\n", &outs[i].m);

        for(j=0; j<outs[i].m; j++)
            fscanf(OutputFile,"%d ", &outs[i].nums[j]);

        fscanf(OutputFile,"%f\n",&outs[i].exec_time);

        fscanf(OutputFile,"%d\n", &outs[i].m);

        fscanf(OutputFile,"%s", outs[i].signal);
        fscanf(OutputFile,"%s", outs[i].sig_rec);

    }
    
    SelectionSortForOuts(outs, n);

#if 0
    for(i=0; i<n; i++)
    {
        printf("%d\n",outs[i].m);
        for(j=0; j<outs[i].m; j++)
            printf("%d ", outs[i].nums[j]);
        printf("\n");
        printf("%f\n\n\n",outs[i].exec_time);
    }
#endif

    sprintf(FinalOutputFileName, "file/output.txt");
    FILE *FinalOutputFile = fopen(FinalOutputFileName, "w");
    if (FinalOutputFile == NULL) {
        printf("Cannot open Final output file \n");
        return 1;
    }

    for(i=0; i<n; i++){
        fprintf(FinalOutputFile, "%f - ", outs[i].exec_time);
        for(j=0; j<outs[i].m; j++)
            fprintf(FinalOutputFile, "%d ", outs[i].nums[j]);
        fprintf(FinalOutputFile, "- ");
        fprintf(FinalOutputFile, "%s %s \n", outs[i].signal, outs[i].sig_rec);
    }

    return 0;
}


