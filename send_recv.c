#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
   MPI_Init(&argc, &argv);
   int rank, size, message;
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   const int tag=16, sender=0, receiver=1, count=1;
   if (rank==0) {
      message = 10;
      MPI_Send(&message, count, MPI_INT, receiver, tag, MPI_COMM_WORLD);
   }
   if (rank==1) {
      MPI_Recv(&message, count, MPI_INT, sender, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      printf("Process 1 received %d from process 0\n", message);
   }
   MPI_Finalize();
}
