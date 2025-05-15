#include <stdio.h>
#include <limits.h>
#include <stdbool.h>

#define V 10  // Max number of cities

// Function to find the nearest unvisited city
int findNearest(int city, int dist[][V], bool visited[], int n) {
    int nearest = -1;
    int min_dist = INT_MAX;

    for (int i = 0; i < n; i++) {
        if (!visited[i] && dist[city][i] && dist[city][i] < min_dist) {
            min_dist = dist[city][i];
            nearest = i;
        }
    }

    return nearest;
}

// TSP using Nearest Neighbor Approximation
void tspNearestNeighbor(int dist[][V], int n) {
    bool visited[V] = {false};
    int path[V + 1];
    int total_cost = 0;

    int current = 0;
    visited[current] = true;
    path[0] = current;

    for (int i = 1; i < n; i++) {
        int next = findNearest(current, dist, visited, n);
        if (next == -1) break;

        total_cost += dist[current][next];
        current = next;
        visited[current] = true;
        path[i] = current;
    }

    total_cost += dist[current][path[0]];  // return to starting city
    path[n] = path[0];  // Complete the cycle

    printf("Approximate TSP Path (Nearest Neighbor):\n");
    for (int i = 0; i <= n; i++) {
        printf("%d ", path[i]);
    }
    printf("\nTotal Cost: %d\n", total_cost);
}

int main() {
    int n;
    printf("Enter number of cities: ");
    scanf("%d", &n);

    int dist[V][V];
    printf("Enter the distance matrix (%d x %d):\n", n, n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            scanf("%d", &dist[i][j]);

    tspNearestNeighbor(dist, n);

    return 0;
}
