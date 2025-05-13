#include <stdio.h>
#include <stdlib.h>

#define V 5   // Number of vertices
#define E 7   // Number of edges

// Structure to represent an edge
struct Edge {
    int src, dest, weight;
};

// Structure to represent a subset for union-find
struct Subset {
    int parent;
    int rank;
};

// Function to compare edges by weight (used in qsort)
int compareEdges(const void* a, const void* b) {
    struct Edge* e1 = (struct Edge*)a;
    struct Edge* e2 = (struct Edge*)b;
    return e1->weight - e2->weight;
}

// Find function with path compression
int find(struct Subset subsets[], int i) {
    if (subsets[i].parent != i)
        subsets[i].parent = find(subsets, subsets[i].parent);
    return subsets[i].parent;
}

// Union function by rank
void Union(struct Subset subsets[], int x, int y) {
    int xroot = find(subsets, x);
    int yroot = find(subsets, y);

    if (subsets[xroot].rank < subsets[yroot].rank)
        subsets[xroot].parent = yroot;
    else if (subsets[xroot].rank > subsets[yroot].rank)
        subsets[yroot].parent = xroot;
    else {
        subsets[yroot].parent = xroot;
        subsets[xroot].rank++;
    }
}

// Function to run Kruskal's algorithm
void kruskalMST(struct Edge edges[]) {
    struct Edge result[V];  // Store MST result
    int e = 0;              // Count of edges in MST
    int i = 0;              // Index variable

    // Sort all the edges by weight
    qsort(edges, E, sizeof(edges[0]), compareEdges);

    // Create V subsets
    struct Subset subsets[V];
    for (int v = 0; v < V; ++v) {
        subsets[v].parent = v;
        subsets[v].rank = 0;
    }

    // Pick the smallest edge and check if it forms a cycle
    while (e < V - 1 && i < E) {
        struct Edge next = edges[i++];

        int x = find(subsets, next.src);
        int y = find(subsets, next.dest);

        // If not in the same subset, include in MST
        if (x != y) {
            result[e++] = next;
            Union(subsets, x, y);
        }
    }

    // Print the result
    printf("Edge \tWeight\n");
    for (i = 0; i < e; ++i)
        printf("%d - %d \t%d\n", result[i].src, result[i].dest, result[i].weight);
}

int main() {
    // Hardcoded edge list for the graph
    struct Edge edges[E] = {
        {0, 1, 2},
        {0, 3, 6},
        {1, 2, 3},
        {1, 3, 8},
        {1, 4, 5},
        {2, 4, 7},
        {3, 4, 9}
    };

    kruskalMST(edges);
    return 0;
}
