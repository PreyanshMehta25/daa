#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>
#include <string.h>

#define N 10

int p[N+1];
int m[N][N], s[N][N];
double* matrices[N];

// Helper to get next power of 2
int nextPowerOf2(int n) {
    int p = 1;
    while (p < n) p <<= 1;
    return p;
}

// Generate random 0/1 matrix
double* generate_matrix(int rows, int cols) {
    double* mat = (double*)malloc(rows * cols * sizeof(double));
    for (int i = 0; i < rows * cols; i++)
        mat[i] = rand() % 2;
    return mat;
}

// Save matrix to CSV
void save_matrix_to_csv(double* matrix, int rows, int cols, int index) {
    char filename[30];
    sprintf(filename, "matrix_%d.csv", index + 1);
    FILE* fp = fopen(filename, "w");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fprintf(fp, "%.0f", matrix[i * cols + j]);
            if (j != cols - 1) fprintf(fp, ",");
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}

// Add two matrices
void add(double* A, double* B, double* C, int n) {
    for (int i = 0; i < n * n; i++)
        C[i] = A[i] + B[i];
}

// Subtract two matrices
void subtract(double* A, double* B, double* C, int n) {
    for (int i = 0; i < n * n; i++)
        C[i] = A[i] - B[i];
}

// Strassen multiplication
void strassen(double* A, double* B, double* C, int n) {
    if (n <= 2) {  // base case 2x2
        C[0] = A[0]*B[0] + A[1]*B[2];
        C[1] = A[0]*B[1] + A[1]*B[3];
        C[2] = A[2]*B[0] + A[3]*B[2];
        C[3] = A[2]*B[1] + A[3]*B[3];
        return;
    }

    int size = n/2;
    int s = size * size;
    double *A11 = A, *A12 = A + size, *A21 = A + n*size, *A22 = A + n*size + size;
    double *B11 = B, *B12 = B + size, *B21 = B + n*size, *B22 = B + n*size + size;

    double *M1 = malloc(s * sizeof(double));
    double *M2 = malloc(s * sizeof(double));
    double *M3 = malloc(s * sizeof(double));
    double *M4 = malloc(s * sizeof(double));
    double *M5 = malloc(s * sizeof(double));
    double *M6 = malloc(s * sizeof(double));
    double *M7 = malloc(s * sizeof(double));

    double *T1 = malloc(s * sizeof(double));
    double *T2 = malloc(s * sizeof(double));

    // M1 = (A11 + A22)*(B11 + B22)
    add(A11, A22, T1, size);
    add(B11, B22, T2, size);
    strassen(T1, T2, M1, size);

    // M2 = (A21 + A22)*B11
    add(A21, A22, T1, size);
    strassen(T1, B11, M2, size);

    // M3 = A11*(B12 - B22)
    subtract(B12, B22, T1, size);
    strassen(A11, T1, M3, size);

    // M4 = A22*(B21 - B11)
    subtract(B21, B11, T1, size);
    strassen(A22, T1, M4, size);

    // M5 = (A11 + A12)*B22
    add(A11, A12, T1, size);
    strassen(T1, B22, M5, size);

    // M6 = (A21 - A11)*(B11 + B12)
    subtract(A21, A11, T1, size);
    add(B11, B12, T2, size);
    strassen(T1, T2, M6, size);

    // M7 = (A12 - A22)*(B21 + B22)
    subtract(A12, A22, T1, size);
    add(B21, B22, T2, size);
    strassen(T1, T2, M7, size);

    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++) {
            int idx = i * size + j;
            C[i*n + j] = M1[idx] + M4[idx] - M5[idx] + M7[idx];             // C11
            C[i*n + j + size] = M3[idx] + M5[idx];                          // C12
            C[(i+size)*n + j] = M2[idx] + M4[idx];                          // C21
            C[(i+size)*n + j + size] = M1[idx] - M2[idx] + M3[idx] + M6[idx]; // C22
        }

    free(M1); free(M2); free(M3); free(M4); free(M5); free(M6); free(M7);
    free(T1); free(T2);
}

double* multiply_strassen(double* A, double* B, int r1, int c1, int c2) {
    int n = nextPowerOf2(fmax(fmax(r1, c1), c2));
    double* A_pad = calloc(n * n, sizeof(double));
    double* B_pad = calloc(n * n, sizeof(double));
    double* C_pad = calloc(n * n, sizeof(double));

    for (int i = 0; i < r1; i++)
        for (int j = 0; j < c1; j++)
            A_pad[i * n + j] = A[i * c1 + j];

    for (int i = 0; i < c1; i++)
        for (int j = 0; j < c2; j++)
            B_pad[i * n + j] = B[i * c2 + j];

    strassen(A_pad, B_pad, C_pad, n);

    double* result = malloc(r1 * c2 * sizeof(double));
    for (int i = 0; i < r1; i++)
        for (int j = 0; j < c2; j++)
            result[i * c2 + j] = C_pad[i * n + j];

    free(A_pad); free(B_pad); free(C_pad);
    return result;
}

// Matrix Chain Order
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

double* matrix_chain_multiply_strassen(int i, int j) {
    if (i == j)
        return matrices[i];
    double* A = matrix_chain_multiply_strassen(i, s[i][j]);
    double* B = matrix_chain_multiply_strassen(s[i][j]+1, j);
    int r1 = p[i], c1 = p[s[i][j]+1], c2 = p[j+1];
    double* result = multiply_strassen(A, B, r1, c1, c2);
    if (i != s[i][j]) free(A);
    if (s[i][j]+1 != j) free(B);
    return result;
}

int random_power_of_two() {
    return 1 << (rand() % 5 + 1);  // 2^1 to 2^5
}

int main() {
    srand(time(NULL));
    p[0] = random_power_of_two();
    for (int i = 1; i <= N; i++) {
        p[i] = random_power_of_two();
        matrices[i-1] = generate_matrix(p[i-1], p[i]);
        save_matrix_to_csv(matrices[i-1], p[i-1], p[i], i - 1);
    }

    matrix_chain_order();

    printf("Optimal Parenthesization: ");
    print_optimal_parens(0, N-1);
    printf("\n");

    clock_t start = clock();
    double* result = matrix_chain_multiply_strassen(0, N-1);
    clock_t end = clock();
    printf("Time for Strassen optimal multiplication: %.6f sec\n",
           (double)(end - start)/CLOCKS_PER_SEC);

    for (int i = 0; i < N; i++) free(matrices[i]);
    free(result);

    return 0;
}
