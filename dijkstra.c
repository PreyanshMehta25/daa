#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int minDistance(int dist[], bool sptSet[], int n)
{
    int min = INT_MAX, min_index;

    for (int v = 0; v < n; v++)
        if (sptSet[v] == false && dist[v] <= min)
            min = dist[v], min_index = v;

    return min_index;
}

void printSolution(int dist[], int n)
{
    printf("Vertex \t\t Distance from Source\n");
    for (int i = 0; i < n; i++){
        if(dist[i] == INT_MAX)
            printf("%d \t\t\t No Path\n", i, dist[i]);
        else
            printf("%d \t\t\t %d\n", i, dist[i]);
    }
}


void printSolutionDest(int dist[], int s, int d)
{
    if(dist[d] == INT_MAX)
        printf("%d \t\t\t No Path\n", s, dist[d]);
    else
        printf("%d \t\t\t %d\n", s, dist[d]);
        //printf("%d \t\t\t %d\n", s, dist[d]);
}

struct AdjListNode {
    int dest;
    int weight;
    struct AdjListNode* next;
};

struct AdjList {
    struct AdjListNode* head;
};

struct Graph {
    int n;
    struct AdjList* array;
};

struct AdjListNode* newAdjListNode(int dest, int weight)
{
    struct AdjListNode* newNode = (struct AdjListNode*)malloc(sizeof(struct AdjListNode));
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

struct Graph* createGraph(int n)
{
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    graph->n = n;

    graph->array = (struct AdjList*)malloc(n * sizeof(struct AdjList));

    for (int i = 0; i < n; i++) {
        graph->array[i].head = NULL;
    }

    return graph;
}

void addEdge(struct Graph* graph, int src, int dest, int weight){
    struct AdjListNode* newNode = newAdjListNode(dest, weight);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;

    newNode = newAdjListNode(src, weight);
    newNode->next = graph->array[dest].head;
    graph->array[dest].head = newNode;
}
void dijkstra(struct Graph* graph, int src, int n)
{
    int dist[n]; 
    bool sptSet[n];

    for (int i = 0; i < n; i++)
        dist[i] = INT_MAX, sptSet[i] = false;

    dist[src] = 0;

    for (int count = 0; count < n - 1; count++){
        int u = minDistance(dist, sptSet, n);
        sptSet[u] = true;

        struct AdjListNode* temp = graph->array[u].head;
        while (temp != NULL)
        {
            int v = temp->dest;
            if (!sptSet[v] && dist[u] != INT_MAX && dist[u] + temp->weight < dist[v])
                dist[v] = dist[u] + temp->weight;

            temp = temp->next;
        }
    }

    printSolution(dist, n);
}
void dijkstraa(struct Graph* graph, int src, int n, int dest)
{
    int dist[n]; 
    bool sptSet[n];

    for (int i = 0; i < n; i++)
        dist[i] = INT_MAX, sptSet[i] = false;

    dist[src] = 0;

    for (int count = 0; count < n - 1; count++){
        int u = minDistance(dist, sptSet, n);
        sptSet[u] = true;

        struct AdjListNode* temp = graph->array[u].head;
        while (temp != NULL)
        {
            int v = temp->dest;
            if (!sptSet[v] && dist[u] != INT_MAX && dist[u] + temp->weight < dist[v])
                dist[v] = dist[u] + temp->weight;

            temp = temp->next;
        }
    }

    printSolutionDest(dist, src, dest);
}

void generateGraph(struct Graph * graph) {
    int edges = 0;
    printf("Enter number of edges: ");
    scanf("%d", &edges);
    int v1 = 0, v2 = 0, w = 0;
    printf("vertex1 vertex2 weight\n");
    for (int i = 0; i < edges; i++) {
        scanf("%d %d %d", &v1, &v2, &w);
        addEdge(graph, v1, v2,w);
    }
    printf("Graph entered successfully!\n");
}

int main()
{
    printf("enter no. of nodes: ");
    int n = 0;
    scanf("%d", &n);
    struct Graph* graph = createGraph(n);
    generateGraph(graph);
    int s = 0;
    int c = 0;
    int t = 1;
    while(t != 0){
    printf("1. user input 2.single destination 3.all pairs\n");
    scanf("%d", &c);
    switch(c){
        case 1:
            printf("enter Source Node: ");
            scanf("%d",&s);
            dijkstra(graph, s, n);
            break;
        case 2:
            printf("Enter destination node: ");
            int dest;
            scanf("%d", &dest);
            printf("\nFinding shortest paths from all nodes to node %d:\n", dest);
            printf("Vertex \t\t Distance from Source\n");
            for(int i = 0; i < n; i++) {
                if(i != dest) {
                    // printf("\nFrom node %d to node %d:\n", i, dest);
                    dijkstraa(graph, i, n, dest);
                }
            }
            break;
        case 3:
        printf("\nAll pairs shortest paths:\n");
        clock_t start = clock();
        for(int i = 0; i < n; i++) {
            printf("\nShortest paths from node %d:\n", i);
            dijkstra(graph, i, n);
            printf("----------------------------------------\n");
        }
        clock_t end = clock();
        double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
        printf("Time taken for all pairs shortest paths: %.5f seconds\n", time_spent);
        break;
    }
    printf("Do you want to run dijkstra? (yes: 1   no: 0):");
    scanf("%d", &t);

}
    return 0;
}

/*
8 10

0 1 1
0 2 4
1 2 2 
1 4 7
2 3 3
4 5 7
5 3 3
6 7 2
4 6 1
6 5 2


15 18

0 2 8
0 2 03
8 6 1
6 2 20
6 7 6
7 1 8
1 5 13
7 13 6
13 12 9
13 14 10
14 11 11
12 5 12
5 2 7
2 3 14
3 4 4
4 10 9
10 9 5
9 11 13

20 24

0 1 21
0 3 6
0 6 5
6 16 6
6 7 4
3 7 4
1 4 9
4 2 19
4 7 20
4 8 13
8 5 11
5 9 10
9 14 9
16 11 4
11 12 9
12 14 3
14 18 2
16 10 9
10 15 12
15 17 13
15 13 14
10 17 10
17 18 4
17 19 5
*/