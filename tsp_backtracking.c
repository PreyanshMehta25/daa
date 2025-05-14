#include <stdio.h>
#include <limits.h>

#define MAX 20

int tsp(int graph[MAX][MAX], int visited[MAX], int pos, int n, int count, int cost, int start, int *minCost) {
    if (count == n && graph[pos][start]) {
        if (cost + graph[pos][start] < *minCost) {
            *minCost = cost + graph[pos][start];
        }
        return *minCost;
    }

    for (int i = 0; i < n; i++) {
        if (!visited[i] && graph[pos][i]) {
            visited[i] = 1;
            tsp(graph, visited, i, n, count + 1, cost + graph[pos][i], start, minCost);
            visited[i] = 0;
        }
    }

    return *minCost;
}

int main() {
    int n;
    int graph[MAX][MAX];
    int visited[MAX] = {0};
    int minCost = INT_MAX;

    printf("Enter the number of cities: ");
    scanf("%d", &n);

    printf("Enter the cost matrix (enter 0 if no direct path):\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            scanf("%d", &graph[i][j]);
        }
    }

    visited[0] = 1;
    int result = tsp(graph, visited, 0, n, 1, 0, 0, &minCost);

    printf("Minimum cost to visit all cities: %d\n", result);
    return 0;
}
