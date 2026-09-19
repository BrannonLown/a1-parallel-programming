#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    long n = 100000000;
    int *arr = malloc(n * sizeof(int));
    for (long i = 0; i < n; i++) {
        arr[i] = rand() % 100;
    }

    struct timespec start, end;
    long sum = 0;

    clock_gettime(CLOCK_MONOTONIC, &start);
    for (long i = 0; i < n; i++) {
        sum += arr[i];
    }
    clock_gettime(CLOCK_MONOTONIC, &end);

    double elapsed_seconds = (end.tv_sec - start.tv_sec) 
                            + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("Sum: %ld\n", sum);
    printf("Elapsed time: %f seconds\n", elapsed_seconds);

    free(arr);
    return 0;
}