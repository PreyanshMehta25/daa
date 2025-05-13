#include <stdio.h>
#include <limits.h>

#define MAX 20

int n;
int final_path[MAX];
int visited[MAX];
int min_cost = INT_MAX;

void copyToFinal(int curr_path[]) {
    for (int i = 0; i < n; i++)
        final_path[i] = curr_path[i];
    final_path[n] = curr_path[0];
}

int firstMin(int graph[MAX][MAX], int i) {
    int min = INT_MAX;
    for (int k = 0; k < n; k++)
        if (graph[i][k] && i != k && graph[i][k] < min)
            min = graph[i][k];
    return min;
}

int secondMin(int graph[MAX][MAX], int i) {
    int first = INT_MAX, second = INT_MAX;
    for (int j = 0; j < n; j++) {
        if (i == j || graph[i][j] == 0)
            continue;

        if (graph[i][j] <= first) {
            second = first;
            first = graph[i][j];
        }
        else if (graph[i][j] < second)
            second = graph[i][j];
    }
    return second;
}

void TSPRec(int graph[MAX][MAX], int curr_bound, int curr_weight,
            int level, int curr_path[]) {
    if (level == n) {
        if (graph[curr_path[level - 1]][curr_path[0]] != 0) {
            int curr_res = curr_weight + graph[curr_path[level - 1]][curr_path[0]];

            if (curr_res < min_cost) {
                copyToFinal(curr_path);
                min_cost = curr_res;
            }
        }
        return;
    }

    for (int i = 0; i < n; i++) {
        if (graph[curr_path[level - 1]][i] != 0 && visited[i] == 0) {
            int temp = curr_bound;
            curr_weight += graph[curr_path[level - 1]][i];

            if (level == 1)
                curr_bound -= ((firstMin(graph, curr_path[level - 1]) +
                                firstMin(graph, i)) / 2);
            else
                curr_bound -= ((secondMin(graph, curr_path[level - 1]) +
                                firstMin(graph, i)) / 2);

            if (curr_bound + curr_weight < min_cost) {
                curr_path[level] = i;
                visited[i] = 1;

                TSPRec(graph, curr_bound, curr_weight, level + 1, curr_path);
            }

            curr_weight -= graph[curr_path[level - 1]][i];
            curr_bound = temp;

            for (int j = 0; j < n; j++)
                visited[j] = 0;
            for (int j = 0; j <= level - 1; j++)
                visited[curr_path[j]] = 1;
        }
    }
}

void TSP(int graph[MAX][MAX]) {
    int curr_path[MAX];
    int curr_bound = 0;

    for (int i = 0; i < n; i++)
        curr_bound += (firstMin(graph, i) + secondMin(graph, i));
    curr_bound = (curr_bound & 1) ? (curr_bound / 2) + 1 : curr_bound / 2;

    visited[0] = 1;
    curr_path[0] = 0;

    TSPRec(graph, curr_bound, 0, 1, curr_path);
}

int main() {
    int graph[MAX][MAX];

    printf("Enter the number of cities: ");
    scanf("%d", &n);

    printf("Enter the cost matrix (0 if no path):\n");
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            scanf("%d", &graph[i][j]);

    TSP(graph);

    printf("Minimum cost: %d\n", min_cost);
    printf("Path Taken: ");
    for (int i = 0; i <= n; i++)
        printf("%d ", final_path[i]);

    printf("\n");
    return 0;
}
