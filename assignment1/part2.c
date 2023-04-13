
#include "part1.h"
#include "sort.h"

void signal_handler(int sig);

int sig_received = 999 ;
struct timeval  sig_rec_time;

int fds[100][2];

void main(int argc, char **argv)
{
    
    int file_count = atoi(argv[1]) ;

    pid_t pid[file_count] ;
    int i, child_status;

    signal(SIGUSR1, signal_handler);
    signal(SIGUSR2, signal_handler);

    for(i=0; i<file_count; i++)
    {
        pipe(fds[i]);
    }

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
        if(  (waitpid(pid[i], &child_status, WNOHANG )) == 0 ) {
            if(pid[i]%2 == 0)
                kill(pid[i], SIGUSR2);
            else
                kill(pid[i], SIGUSR1);
        }
        //sleep(1);
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

void signal_handler(int sig) {
    sig_received = sig;
    gettimeofday(&sig_rec_time, NULL);
}

int childFunction(int no)
{
    // Definitions
    char InputFileName[64];
    int nums[100];
    int m, i;

    // Get start time
    struct timeval  tv1, tv2;
    gettimeofday(&tv1, NULL);

    // Create file names
    sprintf(InputFileName,  "file/input%d.txt",  no);

    // Open files
    FILE *InputFile = fopen(InputFileName, "r");
    if (InputFile == NULL){
        printf("Cannot open input file %d \n", no);
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

#if 0   
    for(i=0; i<m; i++)
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

#if 0
    printf ("Total time = %f seconds\n", time_spent);
#endif

    output_t out;
    //  Writes the results to the intermediate output file
    out.m = m;
    memcpy(out.nums, nums, m*sizeof(int));
    out.exec_time = time_spent;
    strcpy(out.signal,  ( (sig_received==10)?("SIGUSR1"):("SIGUSR2") ) );
    sprintf(out.sig_rec, "%f", sig_rec_time.tv_sec + (double)sig_rec_time.tv_usec/1000000) ;

    // Write to pipe
    write(fds[no][1], &out, sizeof(output_t));
    close(fds[no][0]);

    // Close files
    fclose(InputFile);
    
    return 0;
}


int parentFunction(int file_count) {

    output_t outs[file_count];

    int i, j ;
    char FinalOutputFileName[64];
    int nums[100];
    float exec_time;

    for(i=0; i<file_count; i++)
    {
        read(fds[i][0], &outs[i], sizeof(output_t));
        close(fds[i][1]);
    }
    
    SelectionSortForOuts(outs, file_count);

#if 0
    for(i=0; i<file_count; i++)
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
    if (FinalOutputFile == NULL){
        printf("Cannot open Final output file \n");
        return 1;
    }

    for(i=0; i<file_count; i++)
    {
        fprintf(FinalOutputFile, "%f - ", outs[i].exec_time);
        for(j=0; j<outs[i].m; j++)
            fprintf(FinalOutputFile, "%d ", outs[i].nums[j]);
        fprintf(FinalOutputFile, "- ");
        fprintf(FinalOutputFile, "%s %s \n", outs[i].signal, outs[i].sig_rec);
    }

    return 0;
}


