#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define TOTAL_NUMBERS 100000
#define FILENAME "random_numbers.txt"
#define OUTPUT_FILENAME "sorting_times.csv"

// Function to generate random numbers and save to file
void generateRandomNumbers() {
    FILE* file = fopen(FILENAME, "w");
    if (file == NULL) {
        perror("Error opening file for writing");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL)); // Seed random number generator

    for (int i = 0; i < TOTAL_NUMBERS; ++i) {
        fprintf(file, "%d\n", rand() % TOTAL_NUMBERS);
    }

    fclose(file);
}

// Function to read numbers from file into an array
void readNumbersFromFile(int* arr) {
    FILE* file = fopen(FILENAME, "r");
    if (file == NULL) {
        perror("Error opening file for reading");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < TOTAL_NUMBERS; ++i) {
        if (fscanf(file, "%d", &arr[i]) != 1) {
            fprintf(stderr, "Error reading from file\n");
            exit(EXIT_FAILURE);
        }
    }

    fclose(file);
}

// Merge Sort implementation
void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int* L = malloc(n1 * sizeof(int));
    int* R = malloc(n2 * sizeof(int));

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}

void mergeSort(int arr[], int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

// Quick Sort implementation
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// Function to get current time in milliseconds
long long currentTimeMillis() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long long)tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

int main() {
    // Step 1: Generate random numbers and save to file
    generateRandomNumbers();

    // Step 2: Read numbers from file
    int* numbers = malloc(TOTAL_NUMBERS * sizeof(int));
    if (numbers == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    readNumbersFromFile(numbers);

    // Step 3: Prepare output file for timing results
    FILE* outFile = fopen(OUTPUT_FILENAME, "w");
    if (outFile == NULL) {
        perror("Error opening output file");
        free(numbers);
        exit(EXIT_FAILURE);
    }

    fprintf(outFile, "Block Size,Merge Sort Time (ms),Quick Sort Time (ms)\n");

    // Step 4: Measure sorting times for different block sizes
    for (int blockSize = 100; blockSize <= TOTAL_NUMBERS; blockSize += 100) {
        // Create copies of the original array for each sort
        int* arrMerge = malloc(blockSize * sizeof(int));
        int* arrQuick = malloc(blockSize * sizeof(int));
        
        if (arrMerge == NULL || arrQuick == NULL) {
            perror("Memory allocation failed");
            free(numbers);
            fclose(outFile);
            exit(EXIT_FAILURE);
        }

        // Copy the relevant portion of the array
        for (int i = 0; i < blockSize; ++i) {
            arrMerge[i] = numbers[i];
            arrQuick[i] = numbers[i];
        }

        // Measure Merge Sort time
        long long startMerge = currentTimeMillis();
        mergeSort(arrMerge, 0, blockSize - 1);
        long long endMerge = currentTimeMillis();
        long long durationMerge = endMerge - startMerge;

        // Measure Quick Sort time
        long long startQuick = currentTimeMillis();
        quickSort(arrQuick, 0, blockSize - 1);
        long long endQuick = currentTimeMillis();
        long long durationQuick = endQuick - startQuick;

        // Write results to file
        fprintf(outFile, "%d,%lld,%lld\n", blockSize, durationMerge, durationQuick);

        free(arrMerge);
        free(arrQuick);

        // Print progress
        if (blockSize % 10000 == 0) {
            printf("Processed block size: %d\n", blockSize);
        }
    }

    // Cleanup
    free(numbers);
    fclose(outFile);

    printf("\nSorting times saved to %s\n", OUTPUT_FILENAME);
    printf("Use this file to create 2D plots in Excel/LibreOffice.\n");

    // Space complexity analysis
    printf("\nSpace Complexity Analysis:\n");
    printf("Merge Sort: O(n) - Requires additional space for temporary arrays during merging\n");
    printf("Quick Sort: O(log n) - Space used by recursive calls (in-place sorting)\n");
    printf("Note: Quick sort can degrade to O(n) space in worst-case scenarios.\n");

    return 0;
}