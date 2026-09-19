#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>

void zero_matrix(double *C, int M, int N) {
    for (int i = 0; i < M * N; i++) C[i] = 0;
}

void matmul_ijk(double *A, double *B, double *C, int M, int K, int N){
    zero_matrix(C,M,N);
    for (int i = 0; i < M; i++){
        for (int j = 0; j < N; j++){
            double sum = 0;
            for (int k = 0; k < K; k++){
                sum += A[i*K +k] * B[k*N + j];
            }
            C[i*N + j] = sum;
        }
    }
}

void matmul_ikj(double *A, double *B, double *C, int M, int K, int N){
    zero_matrix(C,M,N);
    for (int i = 0; i < M; i++){
        for (int k = 0; k < K; k++){
            for (int j = 0; j < N; j++){
                C[i*N + j] += A[i*K + k] * B[k*N + j];
            }
        }
    }

}

void matmul_jik(double *A, double *B, double *C, int M, int K, int N){
    zero_matrix(C,M,N);
    for (int j = 0; j < N; j++){
        for (int i = 0; i < M; i++){
            double sum = 0;
            for (int k = 0; k < K; k++){
                sum += A[i*K +k] * B[k*N + j];
            }
            C[i*N + j] = sum;
        }
    }
}

void matmul_jki(double *A, double *B, double *C, int M, int K, int N){
    zero_matrix(C,M,N);
    for (int j = 0; j < N; j++){
        for (int k = 0; k < K; k++){
            for (int i = 0; i < M; i++){
                C[i*N + j] += A[i*K + k] * B[k*N + j];
            }
        }
    }
}

void matmul_kij(double *A, double *B, double *C, int M, int K, int N){
    zero_matrix(C,M,N);
    for (int k = 0; k < K; k++){
        for (int i = 0; i < M; i++){
            for (int j = 0; j < N; j++){
                C[i*N + j] += A[i*K + k] * B[k*N + j];
            }
        }
    }
}

void matmul_kji(double *A, double *B, double *C, int M, int K, int N){
    zero_matrix(C,M,N);
    for (int k = 0; k < K; k++){
        for (int j = 0; j < N; j++){
            for (int i = 0; i < M; i++){
                C[i*N + j] += A[i*K + k] * B[k*N + j];
            }
        }
    }
}

int matrices_equal(double *C1, double *C2, int M, int N){
    for (int i = 0; i < M * N; i++) {
        if (C1[i] != C2[i]) return 0;
    }
    return 1;
}

void fill_matrix(double *mat, int rows, int cols) {
    for (int i = 0; i < rows * cols; i++) {
        mat[i] = (double)(1 + rand() % 100);
    }
}

typedef void (*matmul_func)(double*, double*, double*, int, int, int);

void time_one_matmul(double *A, double *B, double *C, int M, int K, int N, matmul_func f, const char *order_label){
    f(A, B, C, M, K, N);
    double times[10];
    double running_total = 0;

    for (int rep = 0; rep < 10; rep++){
        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);
        f(A, B, C, M, K, N);
        clock_gettime(CLOCK_MONOTONIC, &end);
        times[rep] = (end.tv_sec-start.tv_sec) + (end.tv_nsec-start.tv_nsec) / 1e9;
        running_total += C[0];
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
    double rate = 2.0 * M * K * N / avg;

    printf("%s,%d,%d,%d,%f,%f,%f,%f,%f,%f\n", order_label, M, K, N, avg, median, min, max, rate, running_total);
}

int main(void) {
    int shapes[4][3] = {
        {256, 256, 256},
        {512, 512, 512},
        {1024, 1024, 1024},
        {412, 1024, 256}
    };

    matmul_func fns[6] = {matmul_ijk, matmul_ikj, matmul_jik, matmul_jki, matmul_kij, matmul_kji};
    const char *names[6] = {"ijk", "ikj", "jik", "jki","kij","kji"};

    for (int s = 0; s < 4; s++) {
        int M = shapes[s][0];
        int K = shapes[s][1];
        int N = shapes[s][2];

        double *A = malloc(M*K*sizeof(double));
        double *B = malloc(K*N*sizeof(double));
        double *C1 = malloc(M*N*sizeof(double));
        double *C2 = malloc(M*N*sizeof(double));
        fill_matrix(A, M, K);
        fill_matrix(B, K, N);

        fns[0](A, B, C1, M, K, N);
        for (int i = 1; i < 6; i++) {
            fns[i](A, B, C2, M, K, N);
            if (!matrices_equal(C1, C2, M, N)) {
                printf("MISMATCH: %s vs ijk at %dx%dx%d\n",names[i],M,K,N);
            }
        }

        for (int i = 0; i<6;i++){
            time_one_matmul(A, B, C1, M, K, N, fns[i], names[i]);
        }

        free(A);
        free(B);
        free(C1);
        free(C2);
    }
    return 0;
}