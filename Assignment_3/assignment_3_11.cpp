#include <stdio.h>
#include <stdlib.h>
#include<mpi.h>
#include <random>
#include <iostream>
#include <thread>

using namespace std;

int j = 0;
int count = 0;
pthread_mutex_t lock; 


void iittp_barrier(int r, int p, int tid, int numThreads) {
	if(r == 0 && tid == 0) {
        	MPI_Send(&j, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        	MPI_Recv(&j, 1, MPI_INT, p - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    	}
	else if(tid == 0) {
        	MPI_Recv(&j, 1, MPI_INT, r - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        	MPI_Send(&j, 1, MPI_INT, (r + 1) % p, 0, MPI_COMM_WORLD);
	}
    	if(tid == 0) {
    		MPI_Bcast(&j, 1, MPI_INT, 0, MPI_COMM_WORLD);
    	}
    	pthread_mutex_lock(&lock);
    	count++;
    	pthread_mutex_unlock(&lock);
	while((count % numThreads) != 0);
}


void thread_inc(int tid,int &x,int r,int p,int numThreads) {
	cout << "S: Rank: " << r << " tid: " << tid << " x: " << x << endl;
	iittp_barrier(r, p, tid, numThreads);
}

int main(int argc, char **argv)
{
    	int r;
	int p;
    	MPI_Init(&argc, &argv);
    	MPI_Comm_rank(MPI_COMM_WORLD, &r);
    	MPI_Comm_size(MPI_COMM_WORLD, &p);
    	int numThreads = 4;			// Change number of threads & check
    	int x = r + 1;
    	vector<thread> threads;
	cout << "I: Rank " << r << ": " << x << endl;
    	for(int i = 0; i < numThreads; i++) {
        	threads.push_back(thread(thread_inc, i, ref(x), r, p, numThreads));
    	}
    	for(auto &i : threads) {
        	i.join();
    	}
    	cout << "F: Rank " << r << ": " << x << endl;
    	MPI_Finalize();
}
