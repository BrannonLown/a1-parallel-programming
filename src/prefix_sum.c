#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

void prefix_sum_int(int *arr, long n){
    int running_sum = 0;
    for (long i = 0; i < n; i++){
        int current = arr[i];
        arr[i] = running_sum;
        running_sum += current;
    }
}
void prefix_sum_double(double *arr, long n){
    double running_sum = 0;
    for (long i = 0; i < n; i++){
        double current = arr[i];
        arr[i] = running_sum;
        running_sum += current;
    }
}

void time_one_combination_int(int *arr, long n, const char *opt_level_label){
    int *backup = malloc(n * sizeof(int));
    memcpy(backup, arr, n * sizeof(int));

    prefix_sum_int(arr, n);
    memcpy(arr, backup, n * sizeof(int));

    double times[10];
    long running_total = 0;

    for (int i = 0; i < 10; i++){
        memcpy(arr, backup, n * sizeof(int));

        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);
        prefix_sum_int(arr, n);
        clock_gettime(CLOCK_MONOTONIC, &end);

        times[i] = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
        running_total += arr[n-1];
    }
    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++) {
            if (times[j] > times[j + 1]) {
                double temp = times[j];
                times[j] = times[j+1];
                times[j+1] = temp;
            }
        }
    }
    double sum_of_times = 0;
    for (int x = 0; x < 10; x++){
        sum_of_times += times[x];
    }
    double avg = sum_of_times/10;
    double median = (times[4] + times[5]) / 2;
    double min = times[0];
    double max = times[9];
    double rate = (double)n * sizeof(int) / avg;
    printf("int,%s,%ld,%f,%f,%f,%f,%f,%ld\n", opt_level_label, n, avg, median, min, max, rate, running_total);
    free(backup);
}

void time_one_combination_double(double *arr, long n, const char *opt_level_label){
    double *backup = malloc(n * sizeof(double));
    memcpy(backup, arr, n * sizeof(double));

    prefix_sum_double(arr, n);
    memcpy(arr, backup, n * sizeof(double));

    double times[10];
    double running_total = 0;

    for (int i = 0; i < 10; i++){
        memcpy(arr, backup, n * sizeof(double));

        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);
        prefix_sum_double(arr, n);
        clock_gettime(CLOCK_MONOTONIC, &end);

        times[i] = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
        running_total += arr[n-1];
    }
    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++) {
            if (times[j] > times[j + 1]) {
                double temp = times[j];
                times[j] = times[j+1];
                times[j+1] = temp;
            }
        }
    }
    double sum_of_times = 0;
    for (int x = 0; x < 10; x++){
        sum_of_times += times[x];
    }
    double avg = sum_of_times/10;
    double median = (times[4] + times[5]) / 2;
    double min = times[0];
    double max = times[9];
    double rate = (double)n * sizeof(double) / avg;
    printf("double,%s,%ld,%f,%f,%f,%f,%f,%f\n", opt_level_label, n, avg, median, min, max, rate, running_total);
    free(backup);
}

int main(int argc, char *argv[]) {
    const char *opt_label = "unknown";
    if (argc > 1) {
        opt_label = argv[1];
    }
    
    long sizes[] = {1000000, 10000000, 100000000};

    for (int s = 0; s < 3; s++) {
        long n = sizes[s];

        int *int_arr = malloc(n * sizeof(int));
        double *double_arr = malloc(n * sizeof(double));

        for (long i = 0; i < n; i++) {
            int_arr[i] = rand() % 100;
            double_arr[i] = (double)(rand() % 100);
        }

        time_one_combination_int(int_arr, n, opt_label);
        time_one_combination_double(double_arr, n, opt_label);

        free(int_arr);
        free(double_arr);
    }

    return 0;
}