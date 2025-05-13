#include <stdio.h>

#define MAX_ITEMS 100

typedef struct {
    int value;
    int weight;
} Item;

int maxValue = 0;

void knapsackBacktrack(int capacity, Item items[], int n, int currentItem, int currentValue, int currentWeight) {
    if (currentWeight > capacity) {
        return;
    }

    if (currentItem == n) {
        if (currentValue > maxValue) {
            maxValue = currentValue;
        }
        return;
    }

    knapsackBacktrack(capacity, items, n, currentItem + 1, currentValue + items[currentItem].value, currentWeight + items[currentItem].weight);

    knapsackBacktrack(capacity, items, n, currentItem + 1, currentValue, currentWeight);
}

int main() {
    int n, capacity;

    printf("Enter the number of items: ");
    scanf("%d", &n);
    printf("Enter the capacity of the knapsack: ");
    scanf("%d", &capacity);

    Item items[n];

    for (int i = 0; i < n; i++) {
        printf("Enter the value and weight of item %d: ", i + 1);
        scanf("%d %d", &items[i].value, &items[i].weight);
    }

    knapsackBacktrack(capacity, items, n, 0, 0, 0);

    printf("Maximum value in the knapsack: %d\n", maxValue);

    return 0;
}
