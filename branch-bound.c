#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define N 4
#define MAX_NODES 10000

typedef struct Node {
    int mat[N][N];
    int x, y;
    int level;
    int cost;
    struct Node* parent;
} Node;

// Directions: Down, Left, Up, Right
int dx[] = {1, 0, -1, 0};
int dy[] = {0, -1, 0, 1};

int goal[N][N] = {
    {1, 2, 3, 4},
    {5, 6, 7, 8},
    {9,10,11,12},
    {13,14,15, 0}
};

// Heuristic function: Manhattan Distance
int calculateCost(int mat[N][N]) {
    int cost = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (mat[i][j] != 0) {
                int val = mat[i][j] - 1;
                int goalX = val / N;
                int goalY = val % N;
                cost += abs(i - goalX) + abs(j - goalY);
            }
        }
    }
    return cost;
}

// Check bounds
int isValid(int x, int y) {
    return x >= 0 && x < N && y >= 0 && y < N;
}

// Create new node
Node* createNode(int mat[N][N], int x, int y, int newX, int newY, int level, Node* parent) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (!node) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    memcpy(node->mat, mat, sizeof(node->mat));

    // Swap blank with target tile
    int temp = node->mat[x][y];
    node->mat[x][y] = node->mat[newX][newY];
    node->mat[newX][newY] = temp;

    node->x = newX;
    node->y = newY;
    node->level = level;
    node->cost = calculateCost(node->mat) + level;
    node->parent = parent;

    return node;
}

void printMatrix(int mat[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            printf("%2d ", mat[i][j]);
        printf("\n");
    }
    printf("\n");
}

void printPath(Node* node) {
    if (node == NULL)
        return;
    printPath(node->parent);
    printMatrix(node->mat);
}

int isGoal(int mat[N][N]) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (mat[i][j] != goal[i][j])
                return 0;
    return 1;
}

// Priority queue functions
Node* pq[MAX_NODES];
int pqSize = 0;

void insertPQ(Node* node) {
    if (pqSize >= MAX_NODES) {
        printf("Queue overflow\n");
        exit(1);
    }
    pq[pqSize++] = node;
}

Node* extractMinPQ() {
    int minCost = INT_MAX;
    int idx = -1;
    for (int i = 0; i < pqSize; i++) {
        if (pq[i]->cost < minCost) {
            minCost = pq[i]->cost;
            idx = i;
        }
    }
    Node* result = pq[idx];
    for (int i = idx; i < pqSize - 1; i++)
        pq[i] = pq[i + 1];
    pqSize--;
    return result;
}

void solve(int initial[N][N], int x, int y) {
    Node* root = createNode(initial, x, y, x, y, 0, NULL);
    insertPQ(root);

    while (pqSize > 0) {
        Node* min = extractMinPQ();

        if (isGoal(min->mat)) {
            printf("Output - Path to Goal State:\n\n");
            printPath(min);
            return;
        }

        for (int i = 0; i < 4; i++) {
            int newX = min->x + dx[i];
            int newY = min->y + dy[i];

            if (isValid(newX, newY)) {
                Node* child = createNode(min->mat, min->x, min->y, newX, newY, min->level + 1, min);
                insertPQ(child);
            }
        }
    }

    printf("Solution not found within limit.\n");
}

int main() {
    int initial[N][N] = {
        {1, 3, 4, 15},
        {2, 5, 12, 6},
        {7,11, 14, 8},
        {9,10, 13, 0}
    };

    printf("Input - Initial state of 15 Puzzle problem:\n\n");
    printMatrix(initial);

    solve(initial, 3, 3);

    return 0;
}
