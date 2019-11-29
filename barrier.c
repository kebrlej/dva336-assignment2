#include <mpi.h>
#include <stdio.h>

void My_Barrier(MPI_Comm comm) {
   int rank, size, message;
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   const int tag=16, sender=0, receiver=1, count=1;
   if (rank==0) {
      message = 10;
      MPI_Send(&message, count, MPI_INT, rank+1, tag, MPI_COMM_WORLD);
      MPI_Recv(&message, count, MPI_INT, size-1, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Send(&message, count, MPI_INT, rank+1, tag, MPI_COMM_WORLD);      
      MPI_Recv(&message, count, MPI_INT, size-1, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
   }
   if (rank>0) {
      MPI_Recv(&message, count, MPI_INT, (rank-1)%size, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Send(&message, count, MPI_INT, (rank+1)%size, tag, MPI_COMM_WORLD);
      MPI_Recv(&message, count, MPI_INT, (rank-1)%size, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Send(&message, count, MPI_INT, (rank+1)%size, tag, MPI_COMM_WORLD);
   }
   // else{
   // 	  MPI_Recv(&message, count, MPI_INT, rank-1, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
   //    MPI_Send(&message, count, MPI_INT, rank+1, tag, MPI_COMM_WORLD);
   //    MPI_Recv(&message, count, MPI_INT, rank-1, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
   //    MPI_Send(&message, count, MPI_INT, rank+1, tag, MPI_COMM_WORLD);
   // }
}

int main(int argc, char **argv) {
   MPI_Init(&argc, &argv);
   int rank, size;
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   printf("Processor with rank %d out of %d processors : barrier reached!\n", rank, size);
   My_Barrier(MPI_COMM_WORLD);
   printf("Processor with rank %d out of %d processors : barrier passed!\n", rank, size);
   MPI_Finalize();
   return 0;
}