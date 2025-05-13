#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Function to randomly select a pivot
int randomPivot(int arr[], int low, int high) {
    int range = high - low + 1;
    int pivot = rand() % range + low;
    return pivot;
}

int partition(int arr[], int low, int high) {
    // Randomly select pivot
    int pivotIndex = randomPivot(arr, low, high);
    swap(&arr[low], &arr[pivotIndex]);  // Move pivot to the beginning
    int pivot = arr[low];
    int i = low;
    int j = high;

    while (i < j) {
        while (arr[i] <= pivot && i <= high - 1) {
            i++;
        }
        while (arr[j] > pivot && j >= low + 1) {
            j--;
        }
        if (i < j) {
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[low], &arr[j]);
    return j;
}

void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void rev(int arr[], int n) {
    for (int i = 0; i < n / 2; i++) {
        int temp = arr[i];
        arr[i] = arr[n - i - 1];
        arr[n - i - 1] = temp;
    }
}

int main() {
    for (int ind = 100; ind <= 100000; ind += 100) {
        int* arr = (int*)malloc(ind * sizeof(int));
        if (arr == NULL) {
            printf("Memory allocation failed for size %d\n", ind);
            continue;
        }

        for (int i = 0; i < ind; i++) {
            arr[i] = rand() % 1000;  // Random number between 0 and 999
        }

        clock_t start, end;

        double cpu_time_used;

        // Case when random pivot is selected
        start = clock();
        quickSort(arr, 0, ind - 1);
        end = clock();

        // Calculate time taken
        cpu_time_used = (double)(end - start) / CLOCKS_PER_SEC;

        // Output the time complexity
        printf("%d %f\n", ind, cpu_time_used);

        // Free allocated memory
        free(arr);
    }

    return 0;
}


