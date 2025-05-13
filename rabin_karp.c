#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define PRIME 101

long long createHash(char *str, int end, int q) {
    long long hash = 0;
    for (int i = 0; i < end; i++) {
        hash = (hash * 256 + str[i]) % q;
    }
    return hash;
}

long long recalculateHash(char *str, int oldIndex, int newIndex, long long oldHash, int patternLen, int q) {
    long long newHash = (oldHash - str[oldIndex] * ((long long)pow(256, patternLen - 1)) % q + q) % q;
    newHash = (newHash * 256 + str[newIndex]) % q;
    return newHash;
}

int checkEqual(char *text, int start, char *pattern, int patternLen) {
    for (int i = 0; i < patternLen; i++) {
        if (text[start + i] != pattern[i])
            return 0;
    }
    return 1;
}

void RabinKarp(char *text, char *pattern, int q) {
    int textLen = strlen(text);
    int patternLen = strlen(pattern);
    long long patternHash = createHash(pattern, patternLen, q);
    long long textHash = createHash(text, patternLen, q);

    for (int i = 0; i <= textLen - patternLen; i++) {
        if (patternHash == textHash && checkEqual(text, i, pattern, patternLen)) {
            // Match found - ignored for time test
        }
        if (i < textLen - patternLen) {
            textHash = recalculateHash(text, i, i + patternLen, textHash, patternLen, q);
        }
    }
}

char *loadFile(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("File open failed");
        exit(EXIT_FAILURE);
    }
    fseek(fp, 0, SEEK_END);
    long len = ftell(fp);
    rewind(fp);
    char *buffer = malloc(len + 1);
    fread(buffer, 1, len, fp);
    buffer[len] = '\0';
    fclose(fp);
    return buffer;
}

int main() {
    FILE *csv = fopen("results.csv", "w");
    fprintf(csv, "TextFile,PatternSize,TimeTaken\n");

    char textFilename[100];
    char patternFilename[100];

    for (int t = 1; t <= 10; t++) {
        sprintf(textFilename, "text_files/text%d.txt", t);
        char *text = loadFile(textFilename);

        for (int p = 1; p <= 10; p++) {
            int patternLen = p * 10;
            sprintf(patternFilename, "patterns/pattern%d.txt", patternLen);
            char *pattern = loadFile(patternFilename);

            clock_t start = clock();
            RabinKarp(text, pattern, PRIME);
            clock_t end = clock();

            double timeTaken = ((double)(end - start)) / CLOCKS_PER_SEC;

            fprintf(csv, "text%d.txt,%d,%.8f\n", t, patternLen, timeTaken);
            free(pattern);
        }

        free(text);
    }

    fclose(csv);
    printf("✅ Search completed. Results saved in results.csv\n");
    return 0;
}

