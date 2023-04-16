#include "part1.h"
#include "sort.h"

// Declarations
void signal_handler(int sig);
int sig_received = 999;
char timeString[32];

void main(int argc, char **argv){

    // Declarations
    int n = atoi(argv[1]); // file count
    pid_t pid[n] ;
    int i, child_status;

    // Random number seed
    srand(time(0));

    // Set user defined signals
    struct sigaction sa;
	sa.sa_handler = signal_handler;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);

    // Children's duties
    for(i=0; i<n; i++){
        if( (pid[i] = fork()) == 0  ){
            childFunction(i, rand()%7+1);
            exit(100+i);
        }
    }

    // Determine active children and send signals
    for(i=0; i<n; i++){    
        if(  (waitpid(pid[i], &child_status, WNOHANG )) == 0 ){
            if(pid[i]%2 == 0)
                kill(pid[i], SIGUSR2);
            else
                kill(pid[i], SIGUSR1);
        }
    }

    //  Determine terminated children
    for(i=0; i<n; i++){  
        pid_t wpid = wait(&child_status) ;
        if(WIFEXITED(child_status))
            printf("Child %d terminated with exit status %d \n", wpid, WEXITSTATUS(child_status));
        else
            printf("Child %d terminated abnormally\n", wpid);
    }

    // Parent's duties
    parentFunction(n);
    
}

void signal_handler(int sig) {
    sig_received = sig;
    
    // Signal receive time in hh:mm:ss in UTC+3
//     time_t current_time = time(0);
//     struct tm *time_info = localtime(&current_time);
//     sprintf(timeString, "%02d:%02d:%02d", time_info->tm_hour+3, time_info->tm_min, time_info->tm_sec);
		
    struct timeval tv;
    struct timezone tz;
    struct tm *today;
    gettimeofday(&tv,&tz);
    today = localtime(&tv.tv_sec);
    sprintf(timeString, "%02d:%02d:%02d.%d", today->tm_hour+3, today->tm_min, today->tm_sec, tv.tv_usec);
}

int childFunction(int no, int rand17) {

    printf("Child %d started \n", getpid());

    // Declarations
    char InputFileName[64], OutputFileName[64];
    int nums[100];
    int m, i;

    // Get start time
    struct timeval  tv1, tv2;
    gettimeofday(&tv1, NULL);

    // Create file names
    sprintf(InputFileName,  "files/input%d.txt",  no);
    sprintf(OutputFileName, "files/output%d.txt", no);

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

    // Write the results to the intermediate output file
    fprintf(OutputFile, "%d\n", m);
    for(i=0; i<m; i++)
        fprintf(OutputFile, "%d ", nums[i]);
    fprintf(OutputFile, "\n");
    fprintf(OutputFile, "%f \n", time_spent);
    fprintf(OutputFile, "%s %s\n", ( (sig_received==10)?("SIGUSR1"):("SIGUSR2") ) , timeString );

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

    // Read the intermediate files
    for(i=0; i<n; i++){

        sprintf(OutputFileName, "files/output%d.txt", i);
            
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
        fscanf(OutputFile,"%s", outs[i].sig_rec_time);

    }
    
    // Sort wrt exec. times
    SelectionSortForOuts(outs, n);

    sprintf(FinalOutputFileName, "files/output.txt");
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
        fprintf(FinalOutputFile, "%s %s \n", outs[i].signal, outs[i].sig_rec_time);
    }
    
    return 0;

}
