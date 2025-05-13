#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define INF 999999
#define V 10 // Number of nodes

// Function to generate a random graph with weights
void generateGraph(int graph[V][V]) {
    for (int i = 0; i < V; i++) {
        for (int j = i + 1; j < V; j++) {
            int weight = rand() % 100 + 1; // Random weight between 1 and 100
            graph[i][j] = weight;
            graph[j][i] = weight;
        }
    }
}

// Prim's Algorithm
int prim(int graph[V][V]) {
    int parent[V]; // Array to store the MST
    int key[V]; // Key values to pick minimum weight edge
    int mstSet[V]; // To represent the vertices included in MST
    int totalWeight = 0;

    for (int i = 0; i < V; i++) {
        key[i] = INF;
        mstSet[i] = 0;
    }
    
    key[0] = 0;
    parent[0] = -1;

    for (int count = 0; count < V - 1; count++) {
        int minKey = INF, u;
        for (int v = 0; v < V; v++) {
            if (!mstSet[v] && key[v] < minKey) {
                minKey = key[v];
                u = v;
            }
        }
        
        mstSet[u] = 1;

        for (int v = 0; v < V; v++) {
            if (graph[u][v] && !mstSet[v] && graph[u][v] < key[v]) {
                key[v] = graph[u][v];
                parent[v] = u;
            }
        }
    }

    for (int i = 1; i < V; i++) {
        totalWeight += graph[i][parent[i]];
    }

    return totalWeight;
}

// Helper functions for Kruskal's Algorithm
int find(int parent[], int i) {
    if (parent[i] == i)
        return i;
    return find(parent, parent[i]);
}

void unionSet(int parent[], int rank[], int x, int y) {
    int xroot = find(parent, x);
    int yroot = find(parent, y);
    if (rank[xroot] < rank[yroot])
        parent[xroot] = yroot;
    else if (rank[xroot] > rank[yroot])
        parent[yroot] = xroot;
    else {
        parent[yroot] = xroot;
        rank[xroot]++;
    }
}

// Kruskal's Algorithm
int kruskal(int graph[V][V]) {
    struct Edge {
        int u, v, weight;
    } edges[V * (V - 1) / 2];

    int k = 0;
    for (int i = 0; i < V; i++) {
        for (int j = i + 1; j < V; j++) {
            if (graph[i][j] != 0) {
                edges[k].u = i;
                edges[k].v = j;
                edges[k].weight = graph[i][j];
                k++;
            }
        }
    }

    // Sort edges based on weight (Simple bubble sort for demonstration)
    for (int i = 0; i < k - 1; i++) {
        for (int j = 0; j < k - i - 1; j++) {
            if (edges[j].weight > edges[j + 1].weight) {
                struct Edge temp = edges[j];
                edges[j] = edges[j + 1];
                edges[j + 1] = temp;
            }
        }
    }

    int parent[V], rank[V];
    for (int i = 0; i < V; i++) {
        parent[i] = i;
        rank[i] = 0;
    }

    int totalWeight = 0;
    for (int i = 0; i < k; i++) {
        int u = edges[i].u;
        int v = edges[i].v;
        int weight = edges[i].weight;

        int x = find(parent, u);
        int y = find(parent, v);

        if (x != y) {
            totalWeight += weight;
            unionSet(parent, rank, x, y);
        }
    }

    return totalWeight;
}

int main() {
    srand(time(NULL));  // Seed for random number generation

    FILE *file = fopen("output.csv", "w");
    if (!file) {
        printf("Error opening file!\n");
        return 1;
    }

    // Write CSV header
    fprintf(file, "Graph,Prim_Time(ms),Kruskal_Time(ms)\n");

    for (int i = 0; i < 10; i++) {
        int graph[V][V] = {0};
        generateGraph(graph);

        clock_t start, end;
        double primTime, kruskalTime;

        // Test Prim's Algorithm
        start = clock();
        prim(graph);
        end = clock();
        primTime = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;  // Convert to milliseconds

        // Test Kruskal's Algorithm
        start = clock();
        kruskal(graph);
        end = clock();
        kruskalTime = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;  // Convert to milliseconds

        // Write results to CSV
        fprintf(file, "%d,%.4f,%.4f\n", i + 1, primTime, kruskalTime);
    }

    fclose(file);
    printf("Results saved to output.csv\n");
    return 0;
}

