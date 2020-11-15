#include <bits/stdc++.h>
#include <mpi.h>
#include <omp.h>

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
	long lSum = 0;		// local sum
	int nByp;		// n / p

	MPI_Init(&argc, &argv);
	
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	MPI_Comm_rank(MPI_COMM_WORLD, &r);

	if (r == 0) {

		cout << "Enter the number of elements in the array:" << endl;
		cin >> n;

	}
	
	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

	if(p != 1) nByp = n / (p - 1);
	else nByp = n;

	if (r == 0) {
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
			/*
			 * Comment out the following code to see the value
			 *  */
			cout << arr[i] << " ";
		}

		cout << endl;

		/*
		 * Sharing data with other processes
		 * Diving work in a way that workers get n / (p - 1) elemsents and master get n % (p - 1) elements
		 * Zeroth process is master
		 * */

		for (int i = 1; i < p; i++) {
			MPI_Send(&arr[(i - 1) * nByp], nByp, MPI_LONG, i, i, MPI_COMM_WORLD);
		}

		#pragma omp parallel for reduction(+: lSum)
		for (int i = (p - 1) * nByp; i < n; i++) lSum = lSum + arr[i];
		
		long gSum = lSum;		// Global Sum

		for (int i = 1; i < p; i++) {
			MPI_Recv(&lSum, 1, MPI_LONG, i, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			gSum = gSum + lSum;
		}
		cout << "Sum of all elements:" << endl << gSum << endl;

	}
	else {
		long arr[nByp];
		MPI_Recv(arr, nByp, MPI_LONG, 0, r, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		
		#pragma omp parallel for reduction(+: lSum)
		for (int i = 0; i < nByp; i++) lSum = lSum + arr[i];
		
		MPI_Send(&lSum, 1, MPI_LONG, 0, r, MPI_COMM_WORLD);
	}

	MPI_Finalize();
	return 0;
}

