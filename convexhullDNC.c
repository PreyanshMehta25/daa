// 6) Convex Hull using Divide & Conquer approach with timing
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Point {
    int x, y;
} Point;

int compareX(const void* a, const void* b) {
    return ((Point*)a)->x - ((Point*)b)->x;
}

int crossProduct(Point a, Point b, Point c) {
    return (b.x - a.x)*(c.y - a.y) - (b.y - a.y)*(c.x - a.x);
}

void merge(Point* hull, int* size, Point p) {
    while (*size >= 2 && crossProduct(hull[*size-2], hull[*size-1], p) <= 0)
        (*size)--;
    hull[(*size)++] = p;
}

void convexHullDivideConquer(Point* points, int n) {
    if (n < 3) {
        printf("Convex hull is not possible\n");
        return;
    }

    qsort(points, n, sizeof(Point), compareX);

    Point* hull = (Point*)malloc(sizeof(Point) * (2 * n));
    int k = 0;

    for (int i = 0; i < n; i++)
        merge(hull, &k, points[i]);

    int t = k + 1;
    for (int i = n - 2; i >= 0; i--)
        merge(hull, &k, points[i]);

    printf("Convex Hull points:\n");
    for (int i = 0; i < k - 1; i++)
        printf("(%d, %d)\n", hull[i].x, hull[i].y);

    free(hull);
}

int main() {
    int n;
    printf("Enter number of points: ");
    scanf("%d", &n);
    Point* points = (Point*)malloc(sizeof(Point) * n);
    printf("Enter points (x y):\n");
    for (int i = 0; i < n; i++) {
        scanf("%d %d", &points[i].x, &points[i].y);
    }

    clock_t start = clock();
    convexHullDivideConquer(points, n);
    clock_t end = clock();

    double time_taken = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n", time_taken);

    free(points);
    return 0;
}
