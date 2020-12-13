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
	

	MPI_Init(&argc, &argv);
	
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	MPI_Comm_rank(MPI_COMM_WORLD, &r);

	if (r == 0) {

		cout << "Enter the number of elements in the array:" << endl;
		cin >> n;

	}
	
	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);


	/*
	 * Random Number Generator Initialization
	 * */
	typedef chrono :: high_resolution_clock myClock;
	myClock :: time_point start = myClock :: now();
	long arr[n];
	myClock :: time_point end = myClock :: now();
	myClock :: duration d = end - start;
	unsigned int dur = d.count();
	minstd_rand0 gen(dur);
		
	for (int i = 0; i < n; i++) {
		arr[i] = rand1(gen);
	}
	/*
	 * Uncomment the following code to see the original vectors before sharing	
	for (int i = 0; i < p; i++) {
		if (i == r) {
			cout << "Process " << i << endl;
			for (int j = 0; j < n; j++) cout << arr[j] << " ";
			cout << endl << endl;
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}

	*/

	/*
	 * Sharing data 
	 * */
	
	MPI_Send(arr, n, MPI_LONG, (r + 1) % p, (r + 1) % p, MPI_COMM_WORLD);
	
	MPI_Barrier(MPI_COMM_WORLD);
	
	if (!r) MPI_Recv(arr, n, MPI_LONG, p - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	else MPI_Recv(arr, n, MPI_LONG, r - 1, r, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	
	for (int i = 0; i < p; i++) {
		if (i == r) {
			if(!i) cout << "Process " << i << " Data, recived from " << p - 1 << endl;
			else cout << "Process " << i << " Data, recived from " << i - 1 << endl;
			for (int j = 0; j < n; j++) cout << arr[j] << " ";
			cout << endl << endl;
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}
			
	MPI_Finalize();
	return 0;
}

