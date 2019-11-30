#include "common.hpp"

void M_emittor(int first, int last){
	streamelement x;
	int sendWorkTo = first;
	while(true) {
		x.recv(0);
		if(x.is_terminated()) {
			for(int i = first; i<=last; i++){		
				x.send(i);	
			}
			return;
		}
		x.send(sendWorkTo);

		sendWorkTo++;
		if(sendWorkTo > last){
			sendWorkTo = first;
		}
	}
}

void M_collector(int numberOfWorkers){
	streamelement x;
	int numberOfTerminated = 0;
	while(true) {
		x.recv_any();
		if(x.is_terminated()) {
			numberOfTerminated++;
			if(numberOfTerminated == numberOfWorkers){
				x.send(7);
				return;	
			}
		}
		x.send(7);
	}
}

void M_worker() {
	streamelement x;
	while(true) {
		x.recv(1);
		if(x.is_terminated()) {
			x.send(6);
			return;
		}
		for(int j=0; j<STREAM_ELEMENT_SIZE; ++j)
			x[j] = g1(f1(x[j]));
		x.send(6);
	}
}


#define REQUIRED_COMM_SIZE 8

int main(int argc, char** argv) {
	MPI_Init(&argc, &argv);
	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if(size==REQUIRED_COMM_SIZE) {
		MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
		double start = get_time(MPI_COMM_WORLD);
		if(rank==0) M_first(1);
		if(rank==1) M_emittor(2,5);
		
		if(rank>=2 && rank <= 5) M_worker();

		if(rank==6) M_collector(4);
		if(rank==7) M_last(6);
		double end = get_time(MPI_COMM_WORLD);
		if(rank==0) printf("elapsed time = %.2f seconds\n", end-start);
	} else {
		if(rank==0) printf("run with option '-n %d'\n", REQUIRED_COMM_SIZE);
	}
	MPI_Finalize(); 
	return 0;
}
