#include <stdio.h>
#include <stdlib.h>

int isSafe(int board[], int row, int col) {
    for (int i = 0; i < row; i++) {
        if (board[i] == col || abs(board[i] - col) == abs(i - row))
            return 0;
    }
    return 1;
}

void printBoard(int board[], int n) {
    printf("\nSolution:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (board[i] == j)
                printf(" Q ");
            else
                printf(" . ");
        }
        printf("\n");
    }
}

int solveNQueen(int board[], int row, int n) {
    if (row == n) {
        printBoard(board, n);
        return 1;
    }

    int foundSolution = 0;
    for (int col = 0; col < n; col++) {
        if (isSafe(board, row, col)) {
            board[row] = col;
            foundSolution |= solveNQueen(board, row + 1, n);
        }
    }
    return foundSolution;
}

int main() {
    int n;
    printf("Enter the value of N: ");
    scanf("%d", &n);

    int *board = (int *)malloc(n * sizeof(int));
    if (!solveNQueen(board, 0, n))
        printf("No solution exists for %d-Queens.\n", n);
    free(board);
    return 0;
}
