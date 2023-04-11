#include <unistd.h>
#include <stdio.h>

int main()
{
    int pid = fork();

    if(pid == 0)
        printf("child\n");
    else
        printf("parent %d \n", pid);



    return -1;
}