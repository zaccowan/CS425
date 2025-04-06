#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    // Initialize MPI
    MPI_Init(&argc, &argv);

    // Get process rank and size
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Define n (total numbers to sum)
    int n = 1000000;  // You can change this value
    int local_n = n / size;  // Number of elements each process will handle
    int* local_arr = NULL;  // Local array for each process

    double start_time = MPI_Wtime();

    // Create and initialize the main array only in process 0
    int* main_arr = NULL;
    if (rank == 0) {
        main_arr = (int*)malloc(n * sizeof(int));
        for (int i = 0; i < n; i++) {
            main_arr[i] = i + 1;
        }
    }

    // Calculate distribution
    int remainder = n % size;
    int my_count = local_n + (rank < remainder ? 1 : 0);

    // Arrays needed for scatterv (only on root)
    int *sendcounts = NULL;
    int *displs = NULL;
    if (rank == 0) {
        sendcounts = (int*)malloc(size * sizeof(int));
        displs = (int*)malloc(size * sizeof(int));

        // Calculate send counts and displacements
        int offset = 0;
        for (int i = 0; i < size; i++) {
            sendcounts[i] = local_n + (i < remainder ? 1 : 0);
            displs[i] = offset;
            offset += sendcounts[i];
        }
    }

    // Allocate receive buffer based on my_count
    local_arr = (int*)malloc(my_count * sizeof(int));

    // Use Scatterv to distribute data
    MPI_Scatterv(main_arr,        // Send buffer
        sendcounts,       // Array of send counts
        displs,          // Array of displacements
        MPI_INT,         // Send type
        local_arr,       // Receive buffer
        my_count,        // Receive count
        MPI_INT,         // Receive type
        0,               // Root process
        MPI_COMM_WORLD); // Communicator

    // Calculate local sum
    long long local_sum = 0;
    for (int i = 0; i < my_count; i++) {
    local_sum += local_arr[i];
    }

    // Reduce all local sums to get total sum
    long long total_sum = 0;
    MPI_Reduce(&local_sum, &total_sum, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    double end_time = MPI_Wtime();

    // Print results
    if (rank == 0) {
    printf("Total sum via processes: %lld\n", total_sum);
    printf("Expected sum via Gauss’s formula: %lld\n", (long long)n * (n + 1) / 2);
    printf("Wall Time: %f seconds\n", end_time - start_time);
    }

    // Additional cleanup
    if (rank == 0) {
    free(sendcounts);
    free(displs);
    }

    // Clean up
    free(local_arr);
    if (rank == 0) {
    free(main_arr);
    }

    // Make sure all processes are done before finalizing
    MPI_Barrier(MPI_COMM_WORLD);

    // Finalize MPI
    MPI_Finalize();
    return 0;
}


