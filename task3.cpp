#include "common.hpp"

void M_f() {
	streamelement x;
	while(true) {
		x.recv(0);
		if(x.is_terminated()) {
			x.send(2);
			return;
		}
		for(int j=0; j<STREAM_ELEMENT_SIZE; ++j)
			x[j] = f1(x[j]);
		x.send(2);
	}
}

void M_g(){
	streamelement x;
	while(true) {
		x.recv(1);
		if(x.is_terminated()) {
			x.send(3);
			return;
		}
		for(int j=0; j<STREAM_ELEMENT_SIZE; ++j)
			x[j] = g1(x[j]);
		x.send(3);
	}
}


#define REQUIRED_COMM_SIZE 4

int main(int argc, char** argv) {
	MPI_Init(&argc, &argv);
	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if(size==REQUIRED_COMM_SIZE) {
		MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
		double start = get_time(MPI_COMM_WORLD);
		if(rank==0) M_first(1);
		if(rank==1) M_f();
		if(rank==2) M_g();
		if(rank==3) M_last(2);
		double end = get_time(MPI_COMM_WORLD);
		if(rank==0) printf("elapsed time = %.2f seconds\n", end-start);
	} else {
		if(rank==0) printf("run with option '-n %d'\n", REQUIRED_COMM_SIZE);
	}
	MPI_Finalize(); 
	return 0;
}
