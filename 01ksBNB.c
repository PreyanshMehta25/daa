#include <stdio.h>
#include <stdlib.h>

typedef struct Item {
    int weight, profit;
    float ratio;
} Item;

typedef struct Node {
    int level;
    int profit;
    int weight;
    float bound;
} Node;

int capacity;
int maxProfit = 0;
Item* items;

int compare(const void* a, const void* b) {
    Item* i1 = (Item*)a;
    Item* i2 = (Item*)b;
    return (i2->ratio > i1->ratio) - (i2->ratio < i1->ratio);
}

// Calculate bound for a node
float bound(Node u, int n) {
    if (u.weight >= capacity) return 0;
    float profitBound = u.profit;
    int j = u.level + 1;
    int totWeight = u.weight;

    while (j < n && totWeight + items[j].weight <= capacity) {
        totWeight += items[j].weight;
        profitBound += items[j].profit;
        j++;
    }

    // If there's space, add fraction of next item
    if (j < n)
        profitBound += (capacity - totWeight) * items[j].ratio;

    return profitBound;
}

// Solve using Branch and Bound
void knapsack(int n) {
    Node u, v;
    v.level = -1;
    v.profit = v.weight = 0;
    v.bound = bound(v, n);

    Node queue[1000];
    int front = 0, rear = 0;
    queue[rear++] = v;

    while (front < rear) {
        v = queue[front++];
        if (v.level == n - 1) continue;

        u.level = v.level + 1;

        // Include the item
        u.weight = v.weight + items[u.level].weight;
        u.profit = v.profit + items[u.level].profit;

        if (u.weight <= capacity && u.profit > maxProfit)
            maxProfit = u.profit;

        u.bound = bound(u, n);
        if (u.bound > maxProfit)
            queue[rear++] = u;

        // Exclude the item
        u.weight = v.weight;
        u.profit = v.profit;
        u.bound = bound(u, n);
        if (u.bound > maxProfit)
            queue[rear++] = u;
    }
}

int main() {
    int n;
    printf("Enter number of items: ");
    scanf("%d", &n);

    items = (Item*)malloc(n * sizeof(Item));

    printf("Enter weights and profits of items:\n");
    for (int i = 0; i < n; i++) {
        printf("Item %d (weight profit): ", i + 1);
        scanf("%d %d", &items[i].weight, &items[i].profit);
        items[i].ratio = (float)items[i].profit / items[i].weight;
    }

    printf("Enter knapsack capacity: ");
    scanf("%d", &capacity);

    // Sort items by decreasing ratio
    qsort(items, n, sizeof(Item), compare);

    knapsack(n);
    printf("Maximum profit: %d\n", maxProfit);

    free(items);
    return 0;
}
