#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define INF 1000000
#define MAX 100

typedef struct {
    int u, v, w;
} Edge;

int minDistance(int dist[], int visited[], int n) {
    int min = INF, min_index = -1;
    for (int v = 0; v < n; v++)
        if (!visited[v] && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    return min_index;
}

void dijkstra(int graph[MAX][MAX], int src, int n, int dist[]) {
    int visited[MAX] = {0};
    for (int i = 0; i < n; i++)
        dist[i] = INF;
    dist[src] = 0;

    for (int count = 0; count < n - 1; count++) {
        int u = minDistance(dist, visited, n);
        if (u == -1) break;
        visited[u] = 1;

        for (int v = 0; v < n; v++)
            if (!visited[v] && graph[u][v] != INF &&
                dist[u] + graph[u][v] < dist[v])
                dist[v] = dist[u] + graph[u][v];
    }
}

int bellmanFord(Edge edges[], int m, int n, int h[], int src) {
    for (int i = 0; i < n; i++) h[i] = INF;
    h[src] = 0;

    for (int i = 1; i <= n - 1; i++) {
        for (int j = 0; j < m; j++) {
            int u = edges[j].u;
            int v = edges[j].v;
            int w = edges[j].w;
            if (h[u] != INF && h[u] + w < h[v])
                h[v] = h[u] + w;
        }
    }

    for (int j = 0; j < m; j++) {
        int u = edges[j].u;
        int v = edges[j].v;
        int w = edges[j].w;
        if (h[u] != INF && h[u] + w < h[v])
            return 0; // Negative cycle
    }

    return 1;
}

void johnson(int graph[MAX][MAX], int n) {
    int h[MAX];
    Edge edges[MAX * MAX];
    int m = 0;

    // Build edge list
    for (int u = 0; u < n; u++)
        for (int v = 0; v < n; v++)
            if (graph[u][v] != INF)
                edges[m++] = (Edge){u, v, graph[u][v]};

    // Add dummy vertex q connected to all nodes with 0 weight
    for (int i = 0; i < n; i++)
        edges[m++] = (Edge){n, i, 0};

    if (!bellmanFord(edges, m, n + 1, h, n)) {
        printf("Negative weight cycle detected!\n");
        return;
    }

    // Reweight the original graph
    int newGraph[MAX][MAX];
    for (int u = 0; u < n; u++) {
        for (int v = 0; v < n; v++) {
            if (graph[u][v] != INF)
                newGraph[u][v] = graph[u][v] + h[u] - h[v];
            else
                newGraph[u][v] = INF;
        }
    }

    // Run Dijkstra from each vertex
    for (int u = 0; u < n; u++) {
        int dist[MAX];
        dijkstra(newGraph, u, n, dist);
        printf("Shortest distances from vertex %d:\n", u);
        for (int v = 0; v < n; v++) {
            if (dist[v] != INF)
                printf("To %d: %d\n", v, dist[v] - h[u] + h[v]);
            else
                printf("To %d: INF\n", v);
        }
        printf("\n");
    }
}

int main() {
    int n;
    printf("Enter number of vertices: ");
    scanf("%d", &n);

    int graph[MAX][MAX];
    printf("Enter adjacency matrix (use 999 for INF):\n");
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            scanf("%d", &graph[i][j]);
            if (graph[i][j] == 999) graph[i][j] = INF;
        }

    johnson(graph, n);
    return 0;
}
