#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

int **graph;
int **adj;
int V;

int minKey(int key[], bool mstSet[]) {
    int min = INT_MAX, min_index = -1;

    for (int v = 0; v < V; v++)
        if (!mstSet[v] && key[v] < min)
            min = key[v], min_index = v;

    return min_index;
}

void primMST(int parent[]) {
    int *key = (int *)malloc(V * sizeof(int));
    bool *mstSet = (bool *)malloc(V * sizeof(bool));

    for (int i = 0; i < V; i++)
        key[i] = INT_MAX, mstSet[i] = false;

    key[0] = 0;
    parent[0] = -1;

    for (int count = 0; count < V - 1; count++) {
        int u = minKey(key, mstSet);
        mstSet[u] = true;

        for (int v = 0; v < V; v++)
            if (graph[u][v] && !mstSet[v] && graph[u][v] < key[v])
                parent[v] = u, key[v] = graph[u][v];
    }

    free(key);
    free(mstSet);
}

void buildMSTAdjList(int parent[]) {
    for (int i = 0; i < V; i++)
        for (int j = 0; j < V; j++)
            adj[i][j] = 0;

    for (int i = 1; i < V; i++) {
        adj[i][parent[i]] = 1;
        adj[parent[i]][i] = 1;
    }
}

void DFS(int v, bool visited[], int tour[], int *index) {
    visited[v] = true;
    tour[(*index)++] = v;

    for (int i = 0; i < V; i++) {
        if (adj[v][i] && !visited[i])
            DFS(i, visited, tour, index);
    }
}

void approxTSP() {
    int *parent = (int *)malloc(V * sizeof(int));
    primMST(parent);

    buildMSTAdjList(parent);

    bool *visited = (bool *)calloc(V, sizeof(bool));
    int *tour = (int *)malloc((V + 1) * sizeof(int));
    int index = 0;

    DFS(0, visited, tour, &index);
    tour[index++] = 0;

    printf("\nApproximate TSP tour (using MST + DFS):\n");
    for (int i = 0; i < index; i++)
        printf("%d ", tour[i]);
    printf("\n");

    int cost = 0;
    for (int i = 0; i < index - 1; i++)
        cost += graph[tour[i]][tour[i + 1]];

    printf("Total tour cost: %d\n", cost);

    free(parent);
    free(visited);
    free(tour);
}

int main() {
    printf("Enter number of cities: ");
    scanf("%d", &V);

    graph = (int **)malloc(V * sizeof(int *));
    adj = (int **)malloc(V * sizeof(int *));
    for (int i = 0; i < V; i++) {
        graph[i] = (int *)malloc(V * sizeof(int));
        adj[i] = (int *)malloc(V * sizeof(int));
    }

    printf("Enter the cost matrix (enter 0 if no direct path):\n");
    for (int i = 0; i < V; i++)
        for (int j = 0; j < V; j++)
            scanf("%d", &graph[i][j]);

    approxTSP();

    for (int i = 0; i < V; i++) {
        free(graph[i]);
        free(adj[i]);
    }
    free(graph);
    free(adj);

    return 0;
}
