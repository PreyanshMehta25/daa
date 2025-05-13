#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TEXT_SIZE 10240
#define MAX_PATTERN_SIZE 100
#define NUM_FILES 10
#define d 256
#define q 101

void generateTextFiles(const char *inputFile) {
    FILE *file = fopen(inputFile, "r");
    if (!file) {
        printf("Error opening input file: %s\n", inputFile);
        return;
    }

    char buffer[MAX_TEXT_SIZE + 1];
    size_t bytesRead = fread(buffer, sizeof(char), MAX_TEXT_SIZE, file);
    buffer[bytesRead] = '\0';
    fclose(file);

    srand(time(NULL));

    for (int i = 1; i <= NUM_FILES; i++) {
        int textSize = i * 1024;
        char textFileName[20];
        sprintf(textFileName, "pattern%d.txt", i);

        FILE *textFile = fopen(textFileName, "w");
        if (!textFile) {
            printf("Error creating file: %s\n", textFileName);
            return;
        }

        int startPos = rand() % (bytesRead - textSize + 1);
        fwrite(buffer + startPos, sizeof(char), textSize, textFile);
        fclose(textFile);
        printf("✅ Generated: %-12s | Size: %2d KB | Start Index: %4d\n", textFileName, i, startPos);
    }
}

void rabinKarpSearch(const char *text, const char *pattern, int textSize, int patternSize, FILE *csvFile) {
    if (patternSize == 0) {
        printf("⚠️ Skipping: Pattern size is 0.\n");
        return;
    }

    int m = patternSize, n = textSize;
    int p = 0, t = 0, h = 1;
    int i, j, spuriousHits = 0, actualMatches = 0;
    clock_t start, end;
    double timeTakenMs;

    for (i = 0; i < m - 1; i++)
        h = (h * d) % q;

    for (i = 0; i < m; i++) {
        p = (d * p + pattern[i]) % q;
        t = (d * t + text[i]) % q;
    }

    start = clock();
    for (i = 0; i <= n - m; i++) {
        if (p == t) {
            int match = 1;
            for (j = 0; j < m; j++) {
                if (text[i + j] != pattern[j]) {
                    match = 0;
                    spuriousHits++;
                    break;
                }
            }
            if (match) actualMatches++;
        }

        if (i < n - m) {
            t = (d * (t - text[i] * h) + text[i + m]) % q;
            if (t < 0) t += q;
        }
    }
    end = clock();

    timeTakenMs = (1000.0 * (end - start)) / CLOCKS_PER_SEC;

    printf("Pattern Size: %-3d | Time: %7.3f ms | Matches: %-3d | Spurious: %-4d\n",
           patternSize, timeTakenMs, actualMatches, spuriousHits);

    fprintf(csvFile, "%d,%d,%.3f,%d\n", textSize, patternSize, timeTakenMs, spuriousHits);
}

void processFiles() {
    FILE *csvFile = fopen("execution_times.csv", "w");
    if (!csvFile) {
        printf("Error creating output CSV file.\n");
        return;
    }

    fprintf(csvFile, "TextSize,PatternSize,ExecutionTime(ms),SpuriousHits\n");

    for (int i = 1; i <= NUM_FILES; i++) {
        char textFileName[20];
        sprintf(textFileName, "pattern%d.txt", i);

        FILE *textFile = fopen(textFileName, "r");
        if (!textFile) {
            printf("Error opening: %s\n", textFileName);
            continue;
        }

        fseek(textFile, 0, SEEK_END);
        long textSize = ftell(textFile);
        rewind(textFile);

        char *text = (char *)malloc(textSize + 1);
        if (!text) {
            printf("Memory allocation failed for text\n");
            fclose(textFile);
            continue;
        }

        fread(text, sizeof(char), textSize, textFile);
        text[textSize] = '\0';
        fclose(textFile);

        printf("\nProcessing %s | Size: %ld KB\n", textFileName, textSize / 1024);

        for (int patternSize = 10; patternSize <= 100; patternSize += 10) {
            if (patternSize > textSize) break;

            int startPos = rand() % (textSize - patternSize + 1);
            char pattern[MAX_PATTERN_SIZE + 1];
            strncpy(pattern, text + startPos, patternSize);
            pattern[patternSize] = '\0';

            rabinKarpSearch(text, pattern, textSize, patternSize, csvFile);
        }

        free(text);
    }

    fclose(csvFile);
    printf("\nAll execution times saved in 'execution_times.csv'\n");
}

int main() {
    generateTextFiles("paragraphs.txt");
    processFiles();
    return 0;
}
