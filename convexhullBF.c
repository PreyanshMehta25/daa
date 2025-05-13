// 5) Convex Hull using Brute Force Method
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Point {
    int x, y;
} Point;

// Function to find orientation
int orientation(Point p, Point q, Point r) {
    // Returns:
    // 0 -> colinear
    // >0 -> counterclockwise
    // <0 -> clockwise
    return (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
}

// Function to check if all other points lie on one side of the line
int isEdge(Point a, Point b, Point points[], int n) {
    int pos = 0, neg = 0;
    for (int i = 0; i < n; i++) {
        if ((points[i].x == a.x && points[i].y == a.y) || 
            (points[i].x == b.x && points[i].y == b.y))
            continue;

        int o = orientation(a, b, points[i]);
        if (o > 0)
            pos++;
        else if (o < 0)
            neg++;

        if (pos && neg)
            return 0;  // points on both sides
    }
    return 1;
}

void convexHullBruteForce(Point points[], int n) {
    printf("Convex Hull edges:\n");
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (isEdge(points[i], points[j], points, n)) {
                printf("(%d, %d) -> (%d, %d)\n", points[i].x, points[i].y, points[j].x, points[j].y);
            }
        }
    }
}

int main() {
    int n;
    printf("Enter number of points: ");
    scanf("%d", &n);
    Point points[n];
    printf("Enter points (x y):\n");
    for (int i = 0; i < n; i++) {
        scanf("%d %d", &points[i].x, &points[i].y);
    }

    clock_t start = clock();
    convexHullBruteForce(points, n);
    clock_t end = clock();

    double time_taken = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n", time_taken);

    return 0;
}
