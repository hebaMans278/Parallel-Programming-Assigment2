#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define M 4
#define N 6
#define K 5

void matrix_multiply(int A[M][K], int B[K][N], int C[M][N], int num_threads) {
    #pragma omp parallel num_threads(num_threads)
    {
        int thread_id = omp_get_thread_num();
        int start_row = thread_id * (M / num_threads);
        int end_row = (thread_id == num_threads - 1) ? M - 1 : start_row + (M / num_threads) - 1;

        for (int i = start_row; i <= end_row; i++) {
            for (int j = 0; j < N; j++) {
                C[i][j] = 0;
                for (int k = 0; k < K; k++) {
                    C[i][j] += A[i][k] * B[k][j];
                }
            }
        }
    }
}

int main() {
    int A[M][K] = {{1, 2, 3, 4, 5},
                   {6, 7, 8, 9, 10},
                   {11, 12, 13, 14, 15},
                   {16, 17, 18, 19, 20}};

    int B[K][N] = {{1, 2, 3, 4, 5, 6},
                   {7, 8, 9, 10, 11, 12},
                   {13, 14, 15, 16, 17, 18},
                   {19, 20, 21, 22, 23, 24},
                   {25, 26, 27, 28, 29, 30}};

    int C[M][N] = {{0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0},
                   {0, 0, 0, 0, 0, 0}};

    int num_threads = 2;
    printf("Number of Threads used: %d\n", num_threads);

    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    matrix_multiply(A, B, C, num_threads);

    clock_gettime(CLOCK_MONOTONIC, &end_time);
    double elapsed_time = (end_time.tv_sec - start_time.tv_sec) +
                          (end_time.tv_nsec - start_time.tv_nsec) / 1e9;

    printf("Time taken: %f seconds\n", elapsed_time);

    // print the result matrix
    printf("Result Matrix:\n");
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }

    return 0;
}
