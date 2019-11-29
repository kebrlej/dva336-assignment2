#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
   MPI_Init(&argc, &argv); // init the MPI environment
   int rank, size;
   MPI_Comm_size(MPI_COMM_WORLD, &size); //get the size of the communicator
   MPI_Comm_rank(MPI_COMM_WORLD, &rank); // get the rank of the process
   printf("Hello world from processor with rank %d out of %d processors\n", rank, size);
   MPI_Finalize(); // finalize the MPI environment
}
