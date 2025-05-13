#include <stdio.h>
#include <limits.h>
#include <stdbool.h>

#define V 5  // Number of vertices in the graph

// Function to find the vertex with the minimum key value not yet included in MST
int minKey(int key[], bool mstSet[]) {
    int min = INT_MAX, minIndex;

    for (int v = 0; v < V; v++)
        if (!mstSet[v] && key[v] < min)
            min = key[v], minIndex = v;

    return minIndex;
}

// Function to print the constructed MST
void printMST(int parent[], int graph[V][V]) {
    printf("Edge \tWeight\n");
    for (int i = 1; i < V; i++)
        printf("%d - %d \t%d \n", parent[i], i, graph[i][parent[i]]);
}

// Prim's algorithm to construct and print MST
void primMST(int graph[V][V]) {
    int parent[V];  // Array to store constructed MST
    int key[V];     // Key values to pick minimum weight edge
    bool mstSet[V]; // To represent set of vertices included in MST

    // Initialize all keys as INFINITE
    for (int i = 0; i < V; i++)
        key[i] = INT_MAX, mstSet[i] = false;

    // Include first vertex in MST
    key[0] = 0;     // Make key 0 so that it is picked first
    parent[0] = -1; // First node is always root of MST

    // MST will have V vertices
    for (int count = 0; count < V - 1; count++) {
        int u = minKey(key, mstSet); // Pick the minimum key vertex
        mstSet[u] = true;            // Add the picked vertex to MST

        // Update key and parent for adjacent vertices
        for (int v = 0; v < V; v++)
            if (graph[u][v] && !mstSet[v] && graph[u][v] < key[v])
                parent[v] = u, key[v] = graph[u][v];
    }

    printMST(parent, graph);
}

int main() {
    // Hardcoded graph as adjacency matrix
    int graph[V][V] = {
        {0, 2, 0, 6, 0},
        {2, 0, 3, 8, 5},
        {0, 3, 0, 0, 7},
        {6, 8, 0, 0, 9},
        {0, 5, 7, 9, 0}
    };

    primMST(graph);

    return 0;
}
