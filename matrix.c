#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 5000  // Matrix size N x N

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Calculate local matrix size
    int rows_per_proc = N / size;
    int *A = NULL, *C = NULL;
    int *B = (int*)malloc(N * N * sizeof(int));  // All processes allocate B
    int *local_A = (int*)malloc(rows_per_proc * N * sizeof(int));
    int *local_C = (int*)malloc(rows_per_proc * N * sizeof(int));

    // Process 0 initializes matrices
    if (rank == 0) {
        A = (int*)malloc(N * N * sizeof(int));
        C = (int*)malloc(N * N * sizeof(int));

        srand(time(NULL));
        for (int i = 0; i < N * N; i++) {
            A[i] = rand() % 10;
            B[i] = rand() % 10;  // Initialize B only once
        }
    }

    // Start parallel computation
    double parallel_start = MPI_Wtime();

    // Broadcast B before scatter
    MPI_Bcast(B, N * N, MPI_INT, 0, MPI_COMM_WORLD);

    // Scatter matrix A
    MPI_Scatter(A, rows_per_proc * N, MPI_INT,
                local_A, rows_per_proc * N, MPI_INT,
                0, MPI_COMM_WORLD);

    // Zero initialize local_C
    for (int i = 0; i < rows_per_proc * N; i++) {
        local_C[i] = 0;
    }

    // Parallel matrix multiplication
    for (int i = 0; i < rows_per_proc ; i++ ) {
        for (int j = 0 ; j < N; j++) {
            for (int k = 0; k < N; k++) {
                local_C[i * N + j] += local_A[i * N + k] * B[k * N + j];
            }
        }
    }


    // Gather results
    MPI_Gather(local_C, rows_per_proc * N, MPI_INT,
               C, rows_per_proc * N, MPI_INT,
               0, MPI_COMM_WORLD);

    double parallel_time = MPI_Wtime() - parallel_start;

    // Serial computation on process 0
    if (rank == 0) {
        int *serial_C = (int*)malloc(N * N * sizeof(int));
        double serial_start = MPI_Wtime();

        // Serial matrix multiplication
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                serial_C[i * N + j] = 0;
                for (int k = 0; k < N; k++) {
                    serial_C[i * N + j] += A[i * N + k] * B[k * N + j];
                }
            }
        }
        double serial_time = MPI_Wtime() - serial_start;

        // Print first few elements for verification
        printf("First parallel results: %d %d %d\n", C[0], C[1], C[2]);
        printf("First serial results: %d %d %d\n", serial_C[0], serial_C[1], serial_C[2]);

        // Verify results
        int match = 1;
        for (int i = 0; i < N * N; i++) {
            if (C[i] != serial_C[i]) {
                match = 0;
                printf("Mismatch at index %d: parallel=%d, serial=%d\n", i, C[i], serial_C[i]);
                break;
            }
        }

        printf("Matrix size: %d x %d\n", N, N);
        printf("Number of processes: %d\n", size);
        printf("Parallel time: %f seconds\n", parallel_time);
        printf("Serial time: %f seconds\n", serial_time);
        printf("Speedup: %f\n", serial_time/parallel_time);
        printf("Results match: %s\n", match ? "Yes" : "No");

        free(serial_C);
    }

    // Cleanup
    free(local_A);
    free(local_C);
    free(B);  // All processes free B
    if (rank == 0) {
        free(A);
        free(C);
    }

    MPI_Barrier(MPI_COMM_WORLD);  // Ensure all processes finish before exiting

    MPI_Finalize();
    return 0;
}