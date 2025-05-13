#include <stdio.h>
#include <math.h>

// Function prototypes
double n_cubed(int n);
double log2_n(int n);
double n_times_2_pow_n(int n);
double ln_n(int n);
double two_pow_log2_n(int n);
double identity(int n);
double two_pow_n(int n);
double n_log2_n(int n);
double sqrt_log2_n(int n);
double factorial(int n);

int main() {
    FILE *fp = fopen("function_values.csv", "w");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    // Print header
    printf("n, n^3, lg n, n*2^n, ln n, 2^(lg n), n, 2^n, n lg n, sqrt(lg n), n!\n");
    fprintf(fp, "n,n^3,lg n,n*2^n,ln n,2^(lg n),n,2^n,n lg n,sqrt(lg n),n!\n");

    // Calculate and print values for n from 0 to 100
    for (int n = 0; n <= 100; n++) {
        printf("%d, ", n);
        fprintf(fp, "%d,", n);

        // n^3
        printf("%.2f, ", n_cubed(n));
        fprintf(fp, "%.2f,", n_cubed(n));

        // lg n
        if (n > 0) {
            printf("%.2f, ", log2_n(n));
            fprintf(fp, "%.2f,", log2_n(n));
        } else {
            printf("undef, ");
            fprintf(fp, "undef,");
        }

        // n*2^n
        printf("%.2f, ", n_times_2_pow_n(n));
        fprintf(fp, "%.2f,", n_times_2_pow_n(n));

        // ln n
        if (n > 0) {
            printf("%.2f, ", ln_n(n));
            fprintf(fp, "%.2f,", ln_n(n));
        } else {
            printf("undef, ");
            fprintf(fp, "undef,");
        }

        // 2^(lg n)
        if (n > 0) {
            printf("%.2f, ", two_pow_log2_n(n));
            fprintf(fp, "%.2f,", two_pow_log2_n(n));
        } else {
            printf("undef, ");
            fprintf(fp, "undef,");
        }

        // n
        printf("%.2f, ", identity(n));
        fprintf(fp, "%.2f,", identity(n));

        // 2^n
        printf("%.2f, ", two_pow_n(n));
        fprintf(fp, "%.2f,", two_pow_n(n));

        // n lg n
        if (n > 0) {
            printf("%.2f, ", n_log2_n(n));
            fprintf(fp, "%.2f,", n_log2_n(n));
        } else {
            printf("undef, ");
            fprintf(fp, "undef,");
        }

        // sqrt(lg n)
        if (n > 1) { // sqrt(0) is 0, but sqrt(undef) is undef
            printf("%.2f, ", sqrt_log2_n(n));
            fprintf(fp, "%.2f,", sqrt_log2_n(n));
        } else if (n == 1) {
            printf("0.00, ");
            fprintf(fp, "0.00,");
        } else {
            printf("undef, ");
            fprintf(fp, "undef,");
        }

        // n! (only up to 20)
        if (n <= 20) {
            printf("%.2f", factorial(n));
            fprintf(fp, "%.2f", factorial(n));
        } else {
            printf("ovrflw");
            fprintf(fp, "ovrflw");
        }

        printf("\n");
        fprintf(fp, "\n");
    }

    fclose(fp);
    printf("\nResults saved to function_values.csv\n");
    printf("Use this file to create 2D plots in Excel/LibreOffice.\n");

    return 0;
}

// Function definitions
double n_cubed(int n) {
    return pow(n, 3);
}

double log2_n(int n) {
    return log2(n);
}

double n_times_2_pow_n(int n) {
    return n * pow(2, n);
}

double ln_n(int n) {
    return log(n);
}

double two_pow_log2_n(int n) {
    return pow(2, log2(n));
}

double identity(int n) {
    return n;
}

double two_pow_n(int n) {
    return pow(2, n);
}

double n_log2_n(int n) {
    return n * log2(n);
}

double sqrt_log2_n(int n) {
    return sqrt(log2(n));
}

double factorial(int n) {
    if (n == 0) return 1;
    double result = 1;
    for (int i = 1; i <= n; i++) {
        result *= i;
    }
    return result;
}