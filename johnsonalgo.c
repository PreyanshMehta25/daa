#include <stdio.h>
#include <limits.h>
#include <stdbool.h>

#define V 20 // Number of vertices in the graph

// Function to find the maximum possible flow using BFS
bool bfs(int residualGraph[V][V], int s, int t, int parent[]) {
    bool visited[V] = { false };

    int queue[V];
    int front = 0, rear = 0;
    queue[rear++] = s;
    visited[s] = true;
    parent[s] = -1;

    while (front < rear) {
        int u = queue[front++];
        for (int v = 0; v < V; v++) {
            if (!visited[v] && residualGraph[u][v] > 0) {
                queue[rear++] = v;
                parent[v] = u;
                visited[v] = true;
            }
        }
    }

    return visited[t];
}

// Ford-Fulkerson algorithm
int fordFulkerson(int graph[V][V], int s, int t) {
    int u, v;
    int residualGraph[V][V];

    for (u = 0; u < V; u++)
        for (v = 0; v < V; v++)
            residualGraph[u][v] = graph[u][v];

    int parent[V];
    int maxFlow = 0;

    while (bfs(residualGraph, s, t, parent)) {
        int pathFlow = INT_MAX;

        for (v = t; v != s; v = parent[v]) {
            u = parent[v];
            if (residualGraph[u][v] < pathFlow)
                pathFlow = residualGraph[u][v];
        }

        for (v = t; v != s; v = parent[v]) {
            u = parent[v];
            residualGraph[u][v] -= pathFlow;
            residualGraph[v][u] += pathFlow;
        }

        maxFlow += pathFlow;
    }

    return maxFlow;
}

int main() {
    int graph[V][V] = {0}; // Initialize graph with 0 capacities

    // Hardcoded edges based on the image
    graph[0][1] = 16;
    graph[0][2] = 13;
    graph[1][2] = 10;
    graph[1][3] = 12;
    graph[2][1] = 4;
    graph[2][4] = 14;

    int source = 0;
    int sink = 4;

    int maxFlow = fordFulkerson(graph, source, sink);

    printf("\nMaximum Flow from source %d to sink %d: %d\n", source, sink, maxFlow);

    return 0;
}
