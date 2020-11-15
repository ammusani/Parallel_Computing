#include <bits/stdc++.h>
#include <mpi.h>
using namespace std;

long rand1 (minstd_rand0 &gen) {
	/*
	 * Will return a number from 0 to 255
	 * */
	 
	long long r1 = gen();
	long long r2 = gen();
	return (abs(r1 * r2) % 256);
}

int main (int argc, char **argv) {

	/*
	 * Variable Declaration
	 * */

	int n;			// Size of the matrix
	int p;			// Number of Processes
	int r;			// Rank of the Process
	int m;			// Number of iterations
	
	MPI_Init(&argc, &argv);
	
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	MPI_Comm_rank(MPI_COMM_WORLD, &r);

	if (r == 0) {
		cout << "Enter the number of elements in array followed by iterations:" << endl;
		cin >> n >>  m;

	}
	
	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
	int arr[n];
	/*
	 * Random Number Generator Initialization
	 * */
	typedef chrono :: high_resolution_clock myClock;
	myClock :: time_point start = myClock :: now();
	myClock :: time_point end;
	myClock :: duration d;
	unsigned int dur;
	
	end = myClock :: now();
	d = end - start;
	dur = d.count();
	minstd_rand0 gen(dur);

	for (int i = 0; i < n; i++) arr[i] = rand1(gen);
	
	/*
	 * Uncomment this part to see original arrays
	 * 
	for (int i = 0; i < p; i++) {
		if (i == r) {
			cout << "Rank: " << i << endl;
			for (int i = 0; i < n; i++) cout << arr[i] << " ";
			cout << endl;
		}
		MPI_Barrier(MPI_COMM_WORLD);
	} 
	/* *
	 * */
	MPI_Barrier(MPI_COMM_WORLD);

	
	int xOld;
	int x = 0;

	while(m--) {
		xOld = x;
		if (xOld == r) {
			x = rand1(gen) % p;
			/* Uncomment this to see each step old & new values
			 * 
			cout << xOld << " " << x << endl;
			 /* *
			 * */
		}
		MPI_Bcast(&x, 1, MPI_INT, xOld, MPI_COMM_WORLD);
		if (x == xOld) continue;
		if (xOld == r) {
			// cout << "Old: " << r << " ";
			MPI_Send(arr, n, MPI_INT, x, x, MPI_COMM_WORLD);
		}
		if (x == r) {
			MPI_Recv(arr, n, MPI_INT, xOld, x, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			//cout << "New: " << r << endl;
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}


	for (int i = 0; i < p; i++) {
		if (i == r) {
			cout << "Rank: " << i << endl;
			for (int i = 0; i < n; i++) cout << arr[i] << " ";
			cout << endl;
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}
	
	MPI_Finalize();
	return 0;
}

