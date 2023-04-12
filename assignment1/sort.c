
#include "part1.h"

void SelectionSort(int * nums, int n)
{
    int i,k, tmp;

    k=0;
    for(k=0; k<(n-1); k++)
    {
        for(i=k+1; i<n; i++){
            if(nums[i]<nums[k])
            {
                tmp = nums[k] ;
                nums[k] = nums[i];
                nums[i] = tmp ;
            } 
        }
    }
    return;
}

void InsertionSort(int * nums, int n)
{
    int i ;
    for (int i = 1; i < n; i++) 
    {
        int key = nums[i];
        int j = i - 1;

        while (key < nums[j] && j >= 0) {
            nums[j + 1] = nums[j];
            --j;
        }
        nums[j + 1] = key;
  }

}


void SelectionSortForOuts(output_t * outs, int n)
{
    int i,k;
    output_t tmp;

    k=0;
    for(k=0; k<(n-1); k++)
    {
        for(i=k+1; i<n; i++){
            if(outs[i].exec_time < outs[k].exec_time)
            {
                tmp = outs[k] ;
                outs[k] = outs[i];
                outs[i] = tmp ;
            } 
        }
    }
    return;
}