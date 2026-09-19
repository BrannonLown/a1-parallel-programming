#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

void merge_once(int *arr, int *temp, int lo, int mid, int hi) {
    int i = lo;
    int j = mid;
    int k = lo;
    while(i < mid && j < hi){
        temp[k++] = (arr[i] <= arr[j]) ? arr[i++] : arr[j++];
    }
    while (i < mid){
        temp[k++] = arr[i++];
    } 
    while (j < hi) {
        temp[k++] = arr[j++];
    }
    for (int x = lo; x < hi; x++){
        arr[x] = temp[x];
    }
}

void mergesort_once(int *arr, int *temp, int lo, int hi){
    if (hi - lo <=1){
        return;
    }
    int mid = lo + (hi - lo) / 2;
    mergesort_once(arr, temp, lo, mid);
    mergesort_once(arr, temp, mid, hi);
    merge_once(arr, temp, lo, mid, hi);
}

void mergesort_each(int *arr, int lo, int hi) {
    if (hi - lo <= 1){
        return;
    }
    int mid = lo + (hi - lo) / 2;
    mergesort_each(arr,lo,mid);
    mergesort_each(arr,mid,hi);

    int len = hi-lo;
    int *temp = malloc(len * sizeof(int));
    int i = lo;
    int j = mid;
    int k = 0;
    while (i < mid && j < hi){
        temp[k++] = (arr[i] <= arr[j]) ? arr[i++] : arr[j++];
    }
    while (i < mid){
        temp[k++] = arr[i++];
    } 
    while (j < hi){
        temp[k++] = arr[j++];
    }
    memcpy(arr+lo, temp, len * sizeof(int));
    free(temp);
}

void fill_array(int *arr, long n, const char *order){
    if (strcmp(order, "sorted") == 0){
        for (long i = 0; i < n; i++){
            arr[i] = (int)i;
        } 
    } else if (strcmp(order, "reverse") == 0) {
        for (long i = 0; i < n; i++){
            arr[i] = (int)(n-i);
        }
    } else if (strcmp(order, "equal") == 0){
        for (long i =0; i<n; i++){
            arr[i] = 42;
        }
    } else {
        for (long i = 0; i<n; i++){
            arr[i] = rand() % 1000000;
        }
    }
}

int is_sorted(int *arr, long n) {
    for (long i = 1; i < n; i++){
        if (arr[i-1] > arr[i]){
            return 0;
        }
    }
    return 1;
}

int cmp_int(const void *a, const void *b){
    return (*(int *)a - *(int *)b);
}

void time_one_mergesort(int *arr, int *backup, long n, int *temp, const char *algo, const char *input_label){
    memcpy(arr, backup, n * sizeof(int));
    if (strcmp(algo,"once")==0){
        mergesort_once(arr, temp, 0, n);
    } else if (strcmp(algo, "each")==0){
        mergesort_each(arr,0,n);
    } else{
        qsort(arr, n, sizeof(int), cmp_int);
    }
    memcpy(arr, backup, n * sizeof(int));

    double times[10];
    long running_total = 0;

    for (int rep = 0; rep < 10; rep++){
        memcpy(arr, backup, n * sizeof(int));

        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);
        if (strcmp(algo, "once") ==0){
            mergesort_once(arr, temp, 0, n);
        } else if (strcmp(algo, "each")==0){
            mergesort_each(arr,0,n);
        } else{
           qsort(arr, n, sizeof(int), cmp_int); 
        }
        clock_gettime(CLOCK_MONOTONIC, &end);

        times[rep] = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
        running_total += arr[n-1];
    }

    for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++){
            if (times[j] > times[j+1]){
                double temp_t = times[j];
                times[j] = times[j+1];
                times[j+1] = temp_t;
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
    double rate = (double)n / avg;

    printf("%s,%s,%ld,%f,%f,%f,%f,%f,%ld\n", algo, input_label, n, avg, median, min, max, rate, running_total);
}

int main(void) {
    const char *algos[3] = {"each", "once", "qsort"};
    const char *inputs[4] = {"random", "sorted", "reverse", "equal"};
    long sizes[3] = {1000000, 10000000, 100000000};

    long vn = 5000;
    int *v_arr = malloc(vn * sizeof(int));
    int *v_backup = malloc(vn * sizeof(int));
    int *v_ref = malloc(vn * sizeof(int));
    int *v_temp = malloc(vn * sizeof(int));
    fill_array(v_backup, vn, "random");
    memcpy(v_arr, v_backup, vn * sizeof(int));
    memcpy(v_ref, v_backup, vn * sizeof(int));

    mergesort_once(v_arr, v_temp, 0, vn);
    qsort(v_ref, vn, sizeof(int), cmp_int);

    int ok_sorted = is_sorted(v_arr, vn);
    int ok_same = (memcmp(v_arr, v_ref, vn * sizeof(int)) == 0);
    printf("verify: sorted%s same_as_qsort=%s\n", ok_sorted ? "yes" : "NO", ok_same ? "yes" : "NO");

    free(v_arr); 
    free(v_backup); 
    free(v_ref); 
    free(v_temp);

    for (int a = 0; a < 3; a++){
        for (int in = 0; in < 4; in++){
            for (int s = 0; s < 3; s++){
                long n = sizes[s];
                int *arr = malloc(n * sizeof(int));
                int *backup = malloc(n * sizeof(int));
                int *temp = malloc(n * sizeof(int));

                fill_array(backup, n, inputs[in]);
                time_one_mergesort(arr, backup, n, temp, algos[a], inputs[in]);

                free(arr);
                free(backup);
                free(temp);
            }
        }
    }
    return 0;
}