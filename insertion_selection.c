#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define TOTAL_NUMBERS 100000
#define FILENAME "random_numbers.txt"
#define OUTPUT_FILENAME "sorting_times.csv"

// Function to generate random numbers and save to file
void generateRandomNumbers(const char* filename, int count) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file for writing");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL)); // Seed the random number generator

    for (int i = 0; i < count; ++i) {
        fprintf(file, "%d\n", rand() % 1000000); // Numbers between 0 and 999999
    }

    fclose(file);
}

// Function to read numbers from file into an array
void readNumbersFromFile(const char* filename, int* arr, int count) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file for reading");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < count; ++i) {
        if (fscanf(file, "%d", &arr[i]) != 1) {
            fprintf(stderr, "Error reading from file\n");
            exit(EXIT_FAILURE);
        }
    }

    fclose(file);
}

// Insertion Sort implementation
void insertionSort(int arr[], int n) {
    for (int i = 1; i < n; ++i) {
        int key = arr[i];
        int j = i - 1;

        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// Selection Sort implementation
void selectionSort(int arr[], int n) {
    for (int i = 0; i < n - 1; ++i) {
        int min_idx = i;
        for (int j = i + 1; j < n; ++j) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        // Swap the found minimum element with the first element
        int temp = arr[min_idx];
        arr[min_idx] = arr[i];
        arr[i] = temp;
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
    generateRandomNumbers(FILENAME, TOTAL_NUMBERS);

    // Step 2: Read numbers from file
    int* numbers = (int*)malloc(TOTAL_NUMBERS * sizeof(int));
    if (numbers == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    readNumbersFromFile(FILENAME, numbers, TOTAL_NUMBERS);

    // Step 3: Prepare output file for timing results
    FILE* outFile = fopen(OUTPUT_FILENAME, "w");
    if (outFile == NULL) {
        perror("Error opening output file");
        free(numbers);
        exit(EXIT_FAILURE);
    }

    fprintf(outFile, "Block Size,Insertion Sort Time (ms),Selection Sort Time (ms)\n");

    // Step 4: Measure sorting times for different block sizes
    for (int blockSize = 100; blockSize <= TOTAL_NUMBERS; blockSize += 100) {
        // Create copies of the original array for each sort
        int* arrInsertion = (int*)malloc(blockSize * sizeof(int));
        int* arrSelection = (int*)malloc(blockSize * sizeof(int));
        
        if (arrInsertion == NULL || arrSelection == NULL) {
            perror("Memory allocation failed");
            free(numbers);
            fclose(outFile);
            exit(EXIT_FAILURE);
        }

        // Copy the relevant portion of the array
        for (int i = 0; i < blockSize; ++i) {
            arrInsertion[i] = numbers[i];
            arrSelection[i] = numbers[i];
        }

        // Measure Insertion Sort time
        long long startInsertion = currentTimeMillis();
        insertionSort(arrInsertion, blockSize);
        long long endInsertion = currentTimeMillis();
        long long durationInsertion = endInsertion - startInsertion;

        // Measure Selection Sort time
        long long startSelection = currentTimeMillis();
        selectionSort(arrSelection, blockSize);
        long long endSelection = currentTimeMillis();
        long long durationSelection = endSelection - startSelection;

        // Write results to file
        fprintf(outFile, "%d,%lld,%lld\n", blockSize, durationInsertion, durationSelection);

        free(arrInsertion);
        free(arrSelection);

        // Print progress
        if (blockSize % 10000 == 0) {
            printf("Processed block size: %d\n", blockSize);
        }
    }

    // Cleanup
    free(numbers);
    fclose(outFile);

    printf("Sorting times saved to %s\n", OUTPUT_FILENAME);
    printf("Experiment completed successfully.\n");

    return 0;
}