#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h> 

#define N 10
#define MAX 1024

int p[N+1];
int m[N][N], s[N][N];
double* matrices[N];
int dims[N][2];

int random_power_of_two() {
    return 1 << (rand() % 5 + 1);  // 2^1 to 2^5
}

double* generate_matrix(int rows, int cols) {
    double* mat = (double*)malloc(rows * cols * sizeof(double));
    for (int i = 0; i < rows * cols; i++)
        mat[i] = rand() % 2;
    return mat;
}

void save_matrix_to_csv(double* matrix, int rows, int cols, int index) {
    char filename[30];
    sprintf(filename, "matrix_%d.csv", index + 1);
    FILE* fp = fopen(filename, "w");
    if (!fp) {
        perror("File creation failed");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fprintf(fp, "%.0f", matrix[i * cols + j]);
            if (j != cols - 1) fprintf(fp, ",");
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}

double* multiply(double* A, double* B, int r1, int c1, int c2) {
    double* result = (double*)calloc(r1 * c2, sizeof(double));
    for (int i = 0; i < r1; i++)
        for (int k = 0; k < c1; k++)
            for (int j = 0; j < c2; j++)
                result[i * c2 + j] += A[i * c1 + k] * B[k * c2 + j];
    return result;
}

void matrix_chain_order() {
    for (int i = 0; i < N; i++) m[i][i] = 0;
    for (int l = 2; l <= N; l++) {
        for (int i = 0; i <= N - l; i++) {
            int j = i + l - 1;
            m[i][j] = INT_MAX;
            for (int k = i; k < j; k++) {
                int q = m[i][k] + m[k+1][j] + p[i]*p[k+1]*p[j+1];
                if (q < m[i][j]) {
                    m[i][j] = q;
                    s[i][j] = k;
                }
            }
        }
    }
}

void print_optimal_parens(int i, int j) {
    if (i == j)
        printf("M%d", i+1);
    else {
        printf("(");
        print_optimal_parens(i, s[i][j]);
        printf(" x ");
        print_optimal_parens(s[i][j] + 1, j);
        printf(")");
    }
}

double* matrix_chain_multiply(int i, int j) {
    if (i == j)
        return matrices[i];
    double* A = matrix_chain_multiply(i, s[i][j]);
    double* B = matrix_chain_multiply(s[i][j]+1, j);
    int r1 = p[i], c1 = p[s[i][j]+1], c2 = p[j+1];
    double* result = multiply(A, B, r1, c1, c2);
    if (i != s[i][j]) free(A);
    if (s[i][j]+1 != j) free(B);
    return result;
}

int main() {
    srand(time(NULL));

    // Generate random dimensions
    p[0] = random_power_of_two();
    for (int i = 1; i <= N; i++) {
        p[i] = random_power_of_two();
        dims[i-1][0] = p[i-1];
        dims[i-1][1] = p[i];
        matrices[i-1] = generate_matrix(p[i-1], p[i]);
        save_matrix_to_csv(matrices[i-1], p[i-1], p[i], i - 1);  // Save each matrix
    }

    // Matrix Chain Order
    matrix_chain_order();

    // Print m[i][j]
    printf("Matrix m[i][j] of scalar multiplications:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            if (j < i) printf("   - ");
            else printf("%4d ", m[i][j]);
        printf("\n");
    }

    // Print s[i][j]
    printf("\nMatrix s[i][j] of splits:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            if (j < i) printf("   - ");
            else printf("%4d ", s[i][j]);
        printf("\n");
    }

    // Optimal parenthesization
    printf("\nOptimal Parenthesization:\n");
    print_optimal_parens(0, N-1);
    printf("\n");

    // Multiply and time
    clock_t start = clock();
    double* result = matrix_chain_multiply(0, N-1);
    clock_t end = clock();
    printf("\nTime for regular optimal multiplication: %.6f sec\n",
           (double)(end - start)/CLOCKS_PER_SEC);

    // Cleanup
    for (int i = 0; i < N; i++) free(matrices[i]);
    free(result);

    return 0;
}
