#include <pthread.h>
#include <stdio.h>

int count = 0;
pthread_mutex_t myMutex;

int m;
int matr[100][100];
int d;
int s;


void* doShift(void* p)
{
    int thread_no;
	thread_no = *(int*)p ; //*((int*)p);
    printf("thread_no = %d   ", thread_no);
    int i;

    int elements;
    elements = m/d;
    if(thread_no==(d-1))
        elements += (m%d);

    int start = thread_no * (m/d);
    int end   = thread_no * (m/d) + elements-1;

	pthread_mutex_lock(&myMutex);
	count++;
	pthread_mutex_unlock(&myMutex);

    printf("thread_no = %d , count=%d, no_of_elements=%d : %d -> %d\n", thread_no, count, elements, start, end);

    pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
    int i,j;
   d=5;// printf("thread sayisi: "); scanf("%d", &d);
   s=2;// printf("step sayisi  : "); scanf("%d", &s);

    // Open file
    FILE *InputFile = fopen("input.txt", "r");
    if (InputFile == NULL){
        printf("Cannot open input file \n");
        return 1;
    }

    // Read input file
    fscanf(InputFile,"%d\n",&m);

    for(i=0; i<m; i++){
        for(j=0; j<m; j++){
            fscanf(InputFile,"%02d ", &matr[i][j]);
        }
    }
        
    printf("%d %d \n", matr[2][3], matr[4][9]);

    pthread_t threads[10];

	pthread_mutex_init(&myMutex, NULL);

    int a[d];
    for (i = 0; i < d; i++)
        a[i] = i ;

	for (i = 0; i < d; i++)
		pthread_create(&threads[i], NULL, doShift, &a[i]);

	for (i = 0; i < d; i++)
		pthread_join(threads[i], NULL);

    
    pthread_exit(NULL);

	return 0;
}
