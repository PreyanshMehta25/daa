// 12) Vertex-Cover Problem using Approximation Algorithm
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_VERTICES 20

// Graph structure
typedef struct {
    int adjMatrix[MAX_VERTICES][MAX_VERTICES];
    int V, E;
} Graph;

// Function to create a graph
void createGraph(Graph* g, int vertices, int edges) {
    g->V = vertices;
    g->E = edges;

    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < vertices; j++) {
            g->adjMatrix[i][j] = 0;
        }
    }

    printf("Enter edges in the format (src dest):\n");
    for (int i = 0; i < edges; i++) {
        int src, dest;
        scanf("%d %d", &src, &dest);
        g->adjMatrix[src][dest] = 1;
        g->adjMatrix[dest][src] = 1;
    }
}

// Function to print the vertex cover
void printVertexCover(int cover[], int size) {
    printf("Vertex Cover: { ");
    for (int i = 0; i < size; i++) {
        printf("%d ", cover[i]);
    }
    printf("}\n");
}

// Approximation algorithm for Vertex Cover
void vertexCover(Graph* g) {
    int visited[MAX_VERTICES] = {0}; // To track visited vertices
    int cover[MAX_VERTICES];
    int size = 0;

    for (int u = 0; u < g->V; u++) {
        for (int v = u + 1; v < g->V; v++) {
            // If there is an edge between u and v, and neither u nor v is in the vertex cover
            if (g->adjMatrix[u][v] == 1 && visited[u] == 0 && visited[v] == 0) {
                // Add u and v to the vertex cover
                cover[size++] = u;
                cover[size++] = v;
                visited[u] = 1;
                visited[v] = 1;
                break;
            }
        }
    }

    // Print the vertex cover
    printVertexCover(cover, size);
}

int main() {
    int vertices, edges;
    printf("Enter the number of vertices: ");
    scanf("%d", &vertices);
    printf("Enter the number of edges: ");
    scanf("%d", &edges);

    Graph g;
    createGraph(&g, vertices, edges);

    clock_t start = clock();
    vertexCover(&g);
    clock_t end = clock();

    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("\nTime taken: %f seconds\n", time_taken);

    return 0;
}
