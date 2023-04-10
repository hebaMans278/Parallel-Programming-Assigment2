#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define M 4
#define N 6
#define K 5

struct thread_data {
    int thread_id;
    int num_threads;
    int start_row;
    int end_row;
    int (*A)[K];
    int (*B)[N];
    int (*C)[N];
};

void *matrix_multiply(void *threadarg) {
    struct thread_data *my_data;
    my_data = (struct thread_data *) threadarg;

    int start_row = my_data->start_row;
    int end_row = my_data->end_row;
    int (*A)[K] = my_data->A;
    int (*B)[N] = my_data->B;
    int (*C)[N] = my_data->C;

    for (int i = start_row; i <= end_row; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0;
            for (int k = 0; k < K; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    pthread_exit(NULL);
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
    printf("Number of Threads used: %d", num_threads);
    pthread_t threads[num_threads];
    struct thread_data thread_data_array[num_threads];

    int rows_per_thread = M / num_threads;

    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    int start_row = 0;
    for (int i = 0; i < num_threads; i++) {
        struct thread_data *thread_data = &thread_data_array[i];

        thread_data->thread_id = i;
        thread_data->num_threads = num_threads;

        // divide rows of A into contiguous chunks for each thread
        thread_data->start_row = start_row;
        if (i == num_threads - 1) {
            thread_data->end_row = M - 1;
        } else {
            thread_data->end_row = start_row + rows_per_thread - 1;
        }

        thread_data->A = A;
        thread_data->B = B;
        thread_data->C = C;

        pthread_create(&threads[i], NULL, matrix_multiply, (void *) thread_data);

        start_row = thread_data->end_row + 1;
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

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

