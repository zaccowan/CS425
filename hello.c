#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    // Initialize MPI
    MPI_Init(&argc, &argv);


    // Get process rank and size
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Print rank and total processes
    printf("Hello from process %d out of %d\n", rank, size);

    // Make sure all processes are done before finalizing
    MPI_Barrier(MPI_COMM_WORLD);

    // Finalize MPI
    MPI_Finalize();
    return 0;
}