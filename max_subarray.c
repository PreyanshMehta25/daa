#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <windows.h>
#include <math.h>
#include <float.h>

// Structure to represent a point in 2D space
typedef struct {
    int x, y;
} Point;

// Function to calculate the distance between two points
float calculate_distance(Point a, Point b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

// Compare functions for sorting
int compare_x(const void *a, const void *b) { return ((Point*)a)->x - ((Point*)b)->x; }
int compare_y(const void *a, const void *b) { return ((Point*)a)->y - ((Point*)b)->y; }

// Find the smallest distance in a strip (optimized search)
float strip_closest(Point strip[], int size, float d) {
    float min_dist = d;
    qsort(strip, size, sizeof(Point), compare_y);
    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size && (strip[j].y - strip[i].y) < min_dist; j++) {
            float dist = calculate_distance(strip[i], strip[j]);
            if (dist < min_dist) min_dist = dist;
        }
    }
    return min_dist;
}

// Recursive function to find the smallest distance using divide and conquer
float closest_util(Point points[], int n) {
    if (n <= 3) return FLT_MAX;

    int mid = n / 2;
    Point mid_point = points[mid];

    float left_dist = closest_util(points, mid);
    float right_dist = closest_util(points + mid, n - mid);
    float min_dist = fmin(left_dist, right_dist);

    Point strip[n];
    int j = 0;
    for (int i = 0; i < n; i++) {
        if (fabs(points[i].x - mid_point.x) < min_dist) strip[j++] = points[i];
    }

    return fmin(min_dist, strip_closest(strip, j, min_dist));
}

// Wrapper function to sort points and start divide and conquer
float closest_pair(Point points[], int n) {
    qsort(points, n, sizeof(Point), compare_x);
    return closest_util(points, n);
}

int compare_vals(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int max_subarray_brute(int arr[], int length) {
    int max_sum = arr[0];
    for (int i = 0; i < length; i++) {
        int curr_sum = 0;
        for (int j = i; j < length; j++) {
            curr_sum += arr[j];
            if (curr_sum > max_sum) {
                max_sum = curr_sum;
            }
        }
    }
    return max_sum;
}

int max_subarray_divide_conquer(int arr[], int left, int right) {
    if (left == right) {
        return arr[left];
    }
    int mid = (left + right) / 2;
    int left_max = max_subarray_divide_conquer(arr, left, mid);
    int right_max = max_subarray_divide_conquer(arr, mid + 1, right);

    int cross_sum = arr[mid], temp_sum = arr[mid];
    for (int i = mid - 1; i >= left; i--) {
        temp_sum += arr[i];
        if (temp_sum > cross_sum) cross_sum = temp_sum;
    }
    temp_sum = cross_sum;
    for (int i = mid + 1; i <= right; i++) {
        temp_sum += arr[i];
        if (temp_sum > cross_sum) cross_sum = temp_sum;
    }

    return (left_max > right_max) ? ((left_max > cross_sum) ? left_max : cross_sum)
                                  : ((right_max > cross_sum) ? right_max : cross_sum);
}

int max_subarray_kadane(int arr[], int length) {
    int max_sum = arr[0], current_sum = arr[0];
    for (int i = 1; i < length; i++) {
        current_sum = (current_sum + arr[i] > arr[i]) ? current_sum + arr[i] : arr[i];
        max_sum = (current_sum > max_sum) ? current_sum : max_sum;
    }
    return max_sum;
}

double get_time(LARGE_INTEGER start, LARGE_INTEGER end, LARGE_INTEGER freq) {
    return (double)(end.QuadPart - start.QuadPart) / (double)(freq.QuadPart) * 1e6;
}

int main() {
    FILE *file = fopen("results.csv", "w");
    if (!file) {
        printf("Error opening file for writing!\n");
        return 1;
    }
    fprintf(file, "Block Size, Closest Pair Divide, Max Subarray Brute, Max Subarray Divide, Max Subarray Kadane\n");

    LARGE_INTEGER start, end, freq;
    QueryPerformanceFrequency(&freq);

    int block_sizes[] = {100, 200, 300, 400, 500, 600, 700, 800, 900, 1000};
    int num_sizes = sizeof(block_sizes) / sizeof(block_sizes[0]);

    for (int i = 0; i < num_sizes; i++) {
        int size = block_sizes[i];
        Point *points = (Point *)malloc(size * sizeof(Point));
        int *arr = (int *)malloc(size * sizeof(int));
        if (!arr || !points) {
            printf("Memory allocation failed for size %d\n", size);
            continue;
        }

        for (int j = 0; j < size; j++) {
            arr[j] = rand() % 2001 - 1000;
            points[j].x = rand() % 1000;
            points[j].y = rand() % 1000;
        }

        double time_closest, time_brute, time_divide, time_kadane;

        QueryPerformanceCounter(&start);
        closest_pair(points, size);
        QueryPerformanceCounter(&end);
        time_closest = get_time(start, end, freq);

        QueryPerformanceCounter(&start);
        max_subarray_brute(arr, size);
        QueryPerformanceCounter(&end);
        time_brute = get_time(start, end, freq);

        QueryPerformanceCounter(&start);
        max_subarray_divide_conquer(arr, 0, size - 1);
        QueryPerformanceCounter(&end);
        time_divide = get_time(start, end, freq);

        QueryPerformanceCounter(&start);
        max_subarray_kadane(arr, size);
        QueryPerformanceCounter(&end);
        time_kadane = get_time(start, end, freq);

        fprintf(file, "%d, %.6f, %.6f, %.6f, %.6f\n", size, time_closest, time_brute, time_divide, time_kadane);
        printf("Processed size: %d\n", size);

        free(arr);
        free(points);
    }

    fclose(file);
    printf("Results saved to results.csv\n");
    return 0;
}