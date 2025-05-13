#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include<stdbool.h>
#include <time.h>

typedef struct Edge {
    int src, dest, weight;
} Edge;

typedef struct Graph {
    int V, E; 
    Edge* edge;
} Graph;

Graph* createGraph(int V, int E) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->V = V;
    graph->E = E; // Initialize with the total capacity
    graph->edge = (Edge*)malloc(graph->E * sizeof(Edge));
    return graph;
}

void generateGraph(struct Graph * graph, int edges) {
    int v1 = 0, v2 = 0, w = 0;
    printf("vertex1 vertex2 weight\n");
    for (int i = 0; i < edges; i++) {
        scanf("%d %d %d", &v1, &v2, &w);
        graph->edge[i].src = v1;
        graph->edge[i].dest = v2;
        graph->edge[i].weight = w;
    }
    printf("Graph entered successfully!\n");
}

void printSingleDest(int dist[], int dest) {
    printf("Distance to vertex %d: %d\n", dest, dist[dest]);
}

// Function to print distances from source to all destinations
void printAllDests(int dist[], int n) {
    printf("Vertex   Distance from Source\n");
    for (int i = 0; i < n; ++i)
        printf("%d \t\t %d\n", i, dist[i]);
}

bool BellmanFord(Graph* graph, int src, int dist[]) { 
    int V = graph->V;
    int E = graph->E;

    // Initialize distances from src to all other vertices as INFINITE
    for (int i = 0; i < V; i++)
        dist[i] = INT_MAX;
    dist[src] = 0;

    // Relax all edges V-1 times. A simple shortest path from src to any
    // other vertex can have at-most V-1 edges
    for (int i = 1; i <= V - 1; i++) { 
        for (int j = 0; j < E; j++) {
            int u = graph->edge[j].src;
            int v = graph->edge[j].dest;
            int weight = graph->edge[j].weight;
            if (dist[u] != INT_MAX && dist[u] + weight < dist[v])
                dist[v] = dist[u] + weight;
        }
    }

    // Check for negative-weight cycles.  The above step guarantees shortest
    // distances if graph doesn't contain negative weight cycle.  If we get
    // a shorter path, then there is a cycle.
    for (int i = 0; i < E; i++) {
        int u = graph->edge[i].src;
        int v = graph->edge[i].dest;
        int weight = graph->edge[i].weight;
        if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
            printf("Graph contains negative weight cycle\n");
            return false;
        }
    }
    return true;
}

int main() {
    int V = 0;  // Number of vertices
    int E = 0;  // Number of edges
    printf("Enter number of vertices: ");
    scanf("%d", &V);  
    printf("Enter number of edges: ");
    scanf("%d", &E);
    Graph* graph = createGraph(V, E);
    generateGraph(graph, E);

    int choice, source, destination;
    int dist[V];
    int t = 1;
    while(t != 0) {
        printf("\n=== Bellman-Ford Algorithm Menu ===\n");
        printf("1. Single source and destination\n");
        printf("2. Single destination, all sources\n");
        printf("3. All pairs shortest paths\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter source node: ");
                scanf("%d", &source);
                printf("Enter destination node: ");
                scanf("%d", &destination);
                if (source >= 0 && source < V && destination >= 0 && destination < V) {
                    if(BellmanFord(graph, source, dist)) {
                        printf("\nShortest path from %d to %d: ", source, destination);
                        if(dist[destination] == INT_MAX)
                            printf("No path exists\n");
                        else
                            printf("%d\n", dist[destination]);
                    }
                } else {
                    printf("Invalid source or destination node!\n");
                }
                break;

            case 2:
                printf("Enter destination node: ");
                scanf("%d", &destination);
                if (destination >= 0 && destination < V) {
                    printf("\nShortest paths to node %d:\n", destination);
                    printf("Source\tDistance\n");
                    printf("-------------------\n");
                    for (int i = 0; i < V; i++) {
                        if (i != destination) {
                            if(BellmanFord(graph, i, dist)) {
                                printf("%d\t", i);
                                if(dist[destination] == INT_MAX)
                                    printf("No path\n");
                                else
                                    printf("%d\n", dist[destination]);
                            }
                        }
                    }
                } else {
                    printf("Invalid destination node!\n");
                }
                break;

            case 3:
                printf("\nAll pairs shortest paths:\n");
                clock_t start = clock();
                for (int i = 0; i < V; i++) {
                    if(BellmanFord(graph, i, dist)) {
                        printf("\nFrom source %d:\n", i);
                        printf("Dest\tDistance\n");
                        printf("-------------------\n");
                        
                        for (int j = 0; j < V; j++) {
                            if (i != j) {
                                printf("%d\t", j);
                                if(dist[j] == INT_MAX)
                                    printf("No path\n");
                                else
                                    printf("%d\n", dist[j]);
                            }
                        }
                        
                    }
                }
                clock_t end = clock();
                double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
                printf("Time taken: %.5f seconds\n", time_spent);
                break;
            default:
                printf("Invalid choice!\n");
        }

        printf("\nDo you want to continue? (1 for yes, 0 for no): ");
        scanf("%d", &t);
    }

    free(graph->edge);
    free(graph);
    return 0;
}

/*
8 10

0 1 1
0 2 4
1 2 -2 
1 4 7
2 3 3
4 5 7
5 3 -3
6 7 2
4 6 1
6 5 2


15 
0 1 21
0 3 -6
0 6 5
6 7 4
3 7 -4
6 14 6
0 2 3
1 4 2
2 5 1
3 6 7
4 7 9
8 9 2
9 10 -5
10 11 1
11 12 8
12 13 -3
13 14 7

20 24

0 1 21
0 3 -6
0 6 5
6 16 6
6 7 4
3 7 -4
0 2 3
1 4 2
2 5 1
3 6 7
4 7 -9
8 9 2
9 10 5
10 11 1
11 12 8
12 13 3
13 14 -7
14 15 4
15 16 9
16 17 2
17 18 6
18 19 1
19 17 -3
17 13 5
*/