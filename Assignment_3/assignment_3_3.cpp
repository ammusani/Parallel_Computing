#include <bits/stdc++.h>
#include <mpi.h>
using namespace std;


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
		 * Diving work in a way that workers get n / (p - 1) elemsents and master gets last n % (p - 1) elements
		 * By Master I mean it does collect the data and does all the pre & post sum processing
		 * Zeroth process is master
		 * Assuming Zero indexing
		 * Since elements are arr[i] = i, I have not created an explicit array to reduce the loop for creation
		 * */


		for (int i = (p - 1) * nByp; i < n; i++) lSum = lSum + i;
		
		long sum = lSum;

		for (int i = 1; i < p; i++) {
			MPI_Recv(&lSum, 1, MPI_LONG, i, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			sum = sum + lSum;
		}
		cout << "Sum of all elements:" << endl << sum << endl;

	}
	else {

		for (int i = (r - 1) * nByp; i < r * nByp; i++) lSum = lSum + i;
		MPI_Send(&lSum, 1, MPI_LONG, 0, r, MPI_COMM_WORLD);
	}

	MPI_Finalize();
	return 0;
}

