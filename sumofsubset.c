/*To Implement Sum of subsets Problem using Backtracking Agorithm for the user entered input and then find the 
running time of implementation using time() function.*/

#include<stdio.h>
#include<stdlib.h>
#include<time.h>

void sumofsubset(int arr[], int size, int index, int subset[], int currentsum, int target)
{
    if(currentsum==target){
        printf("{ ");   
        for(int i=0;i<size; i++)
        {
            if(subset[i]!=0){
                printf("%d ",arr[i]);
            }
        }
        printf("} \n");
        return;
    }

    if(currentsum>target || index>=size){
        return;
    }
    subset[index]=arr[index];
    sumofsubset(arr, size, index+1, subset, currentsum+arr[index], target);

    subset[index] = 0;
    sumofsubset(arr, size, index+1, subset, currentsum, target);
}

int main()
{
    int n, target;
    printf("Enter the number of elements in the set\n");
    scanf("%d", &n);
    int arr[n], subset[n];
    for(int i=0; i<n; i++){
        subset[i] = 0;
    }
    printf("Enter the elements of the set\n");
    for(int i=0; i<n; i++)
    {
        scanf("%d", &arr[i]);
    }
    printf("Enter target sum");
    scanf("%d", &target);

    printf("subsets with sum %d are:\n", target);
    clock_t start = clock();
    sumofsubset(arr, n, 0, subset, 0, target);
    clock_t end = clock();

    printf("Time Taken: %f\n", 1000*(double)(end - start) / CLOCKS_PER_SEC);
    return 0;
}