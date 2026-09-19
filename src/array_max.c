#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

typedef int (*max_func_int) (int*, long);
typedef double (*max_func_double) (double*, long);

//Brannon Lown ITCS 4145 001

/* A - branching */          /* B - branchless */
//if (a[i] > m) m = a[i];      m = (a[i] > m) ? a[i] : m;

int find_max_A_int(int *arr, long n) {
    int m = arr[0];
    for (long i = 0; i < n; i++){
        if(arr[i] > m){
            m = arr[i];
        }
    }
    return m;
}

int find_max_B_int(int *arr, long n){
    int m = arr[0];
    for (long i = 0; i < n; i++){
        m = (arr[i] > m) ? arr[i] : m;
    }
    return m;
}

double find_max_A_double(double *arr, long n) {
    double m = arr[0];
    for (long i = 0; i < n; i++){
        if(arr[i] > m){
            m = arr[i];
        }
    }
    return m;
}

double find_max_B_double(double *arr, long n){
    double m = arr[0];
    for (long i = 0; i < n; i++){
        m = (arr[i] > m) ? arr[i] : m;
    }
    return m;
}

void time_one_combination_int(int *arr, long n, max_func_int f, const char *version_label, const char *order_label){
    f(arr, n);
    double times[10];
    long running_total = 0;

    for (int i = 0; i<10; i++){
        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);
        long result = f(arr, n);
        clock_gettime(CLOCK_MONOTONIC, &end);
        times[i] = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
        running_total += result;
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

    printf("int,%s,%s,%ld,%f,%f,%f,%f,%f,%ld\n", version_label, order_label, n, avg, median, min, max, rate, running_total);
}

void time_one_combination_double(double *arr, long n, max_func_double f, const char *version_label, const char *order_label){
    f(arr, n);
    double times[10];
    double running_total = 0;

    for (int i = 0; i<10; i++){
        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);
        double result = f(arr, n);
        clock_gettime(CLOCK_MONOTONIC, &end);
        times[i] = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
        running_total += result;
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

    printf("double,%s,%s,%ld,%f,%f,%f,%f,%f,%f\n", version_label, order_label, n, avg, median, min, max, rate, running_total);
}

void fill_int(int *arr, long n, const char *order) {
    if (strcmp(order, "sorted") == 0) {
        for (long i = 0; i < n; i++) arr[i] = i;
    } else if (strcmp(order, "reverse") == 0) {
        for (long i = 0; i < n; i++) arr[i] = n - i;
    } else {
        for (long i = 0; i < n; i++) arr[i] = rand() % 1000000;
    }
}

void fill_double(double *arr, long n, const char *order) {
    if (strcmp(order, "sorted") == 0) {
        for (long i = 0; i < n; i++) arr[i] = (double)i;
    } else if (strcmp(order, "reverse") == 0) {
        for (long i = 0; i < n; i++) arr[i] = (double)(n - i);
    } else {
        for (long i = 0; i < n; i++) arr[i] = (double)(rand() % 1000000);
    }
}

void run_bandwidth_sweep() {
    long bytes = 2048;
    long max_bytes = 268435456;

    fprintf(stderr, "DEBUG: entered sweep, bytes=%ld\n", bytes);

    while (bytes <= max_bytes) {
        long n = bytes / sizeof(double);
        if (n < 1) n = 1;

        double *arr = malloc(n * sizeof(double));
        fill_double(arr, n, "random");
        time_one_combination_double(arr, n, find_max_B_double, "B", "bandwidth_sweep");
        free(arr);

        bytes *= 2;
    }

    fprintf(stderr, "DEBUG: sweep loop finished\n");
}

int main() {
    long sizes[] = {1000000, 10000000, 100000000};
    const char *orders[] = {"sorted", "reverse", "random"};
    for (int s = 0; s < 3; s++){
        long n = sizes[s];
        int *int_arr = malloc(n * sizeof(int));
        double *double_arr = malloc(n * sizeof(double));

        for (int o = 0; o < 3; o++) {
            fill_int(int_arr, n, orders[o]);
            time_one_combination_int(int_arr, n, find_max_A_int, "A", orders[o]);

            fill_int(int_arr, n, orders[o]);
            time_one_combination_int(int_arr, n, find_max_B_int, "B", orders[o]);

            fill_double(double_arr, n, orders[o]);
            time_one_combination_double(double_arr, n, find_max_A_double, "A", orders[o]);

            fill_double(double_arr, n, orders[o]);
            time_one_combination_double(double_arr, n, find_max_B_double, "B", orders[o]);
        }

        free(int_arr);
        free(double_arr);
    }
    fprintf(stderr, "DEBUG: about to call run_bandwidth_sweep\n");
    run_bandwidth_sweep();
    fprintf(stderr, "DEBUG: finished run_bandwidth_sweep\n");
    return 0;
}