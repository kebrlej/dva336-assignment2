#include "common.hpp"

#define WORKER_ELEMENT_SIZE 2

using workerelement = streamelement_t<WORKER_ELEMENT_SIZE>;

void M_splitter(int first, int last){
	streamelement x;
	int sendWorkTo = first;
	while(true) {
		x.recv(0);
		workerelement y;

		if(x.is_terminated()) {
			y.set_terminated();
			for(int i = first; i<=last; i++){		
				y.send(i);	
			}
			return;
		}

		for(int i = first; i <= last; i++){
			y[0]=x[(i-first)*WORKER_ELEMENT_SIZE];
			y[1]=x[(i-first)*WORKER_ELEMENT_SIZE+1];
			y.send(i);
		}
	}
}

void M_collector(int numberOfWorkers, int first){
	streamelement x;
	workerelement y;
	int numberOfTerminated = 0;
	int numberOfReceived = 0;
	while(true) {
		int senderRank = y.recv_any();
		if(y.is_terminated()) {
			numberOfTerminated++;
			if(numberOfTerminated == numberOfWorkers){
				x.set_terminated();
				x.send(8);
				return;	
			}
		}else{
			x[(senderRank - first)*WORKER_ELEMENT_SIZE] = y[0];
			x[(senderRank - first)*WORKER_ELEMENT_SIZE + 1] = y[1];
			numberOfReceived++;

			if(numberOfReceived == numberOfWorkers){
				numberOfReceived = 0;
				x.send(8);
			}
		}
	
	}
}

void M_worker() {
	workerelement y;
	while(true) {
		y.recv(1);
		if(y.is_terminated()) {
			y.send(7);
			return;
		}
		for(int j=0; j<WORKER_ELEMENT_SIZE; ++j)
			y[j] = g1(f1(y[j]));
		y.send(7);
	}
}


#define REQUIRED_COMM_SIZE 9

int main(int argc, char** argv) {

	MPI_Init(&argc, &argv);
	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if(size==REQUIRED_COMM_SIZE) {
		MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
		double start = get_time(MPI_COMM_WORLD);
		if(rank==0) M_first(1);

		if(rank==1) M_splitter(2,6);
		
		if(rank>=2 && rank <= 6) M_worker();

		if(rank==7) M_collector(5,2);
		if(rank==8) M_last(7);
		double end = get_time(MPI_COMM_WORLD);
		if(rank==0) printf("elapsed time = %.2f seconds\n", end-start);
	} else {
		if(rank==0) printf("run with option '-n %d'\n", REQUIRED_COMM_SIZE);
	}
	MPI_Finalize(); 
	return 0;
}
