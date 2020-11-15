#include <bits/stdc++.h>
#include <mpi.h>
using namespace std;

long blockSum(int r, int nByp) {
	long lSum = 0;
	for (int i = (r - 1) * nByp; i < r * nByp; i++) lSum = lSum + i;
	return lSum;
}

long cyclicSum(int r, int nByp, int p) {
	long lSum = 0;
	int k = p - 1;
	for (int i = r - 1; i < k * nByp; i = i + k) {
		lSum = lSum + i;
	}
	return lSum;
}

long blockCyclicSum(int r, int nByp, int p) {
	long lSum = 0;
	int k = p - 1;
	int l = 4;		// Block size in the cyclic, you can change it to anything
	int kl = k * l;
	int i = (r - 1) * l;
	// cout << i << endl;	
	int m = nByp / l;
	int lmk = l * m * k;
	for (; i < lmk; i = i + kl) {
		for (int j = i; j < i + l; j++) {
			lSum = lSum + j;
			// cout << j << endl;
		}
	}
	m = nByp % l;
	for (i = lmk + (r - 1) * m; i < lmk + r * m; i++) {
		lSum = lSum + i;
		// cout << i << endl;
	}
	return lSum;
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
	int m;			//  1 = Block, 2 = Cyclic, 3 = Block Cyclic

	MPI_Init(&argc, &argv);
	
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	MPI_Comm_rank(MPI_COMM_WORLD, &r);

	if (r == 0) {

		cout << "Enter the number of elements in the array:" << endl;
		cin >> n;
		cout << "Enter 1 for Block, 2 for Cyclic & 3 for Block Cyclic" << endl;
		cin >> m;

	}
	
	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
	
	if(p != 1) nByp = n / (p - 1);
	else nByp = n;

	if (r == 0) {

		/*
		 * Diving work in a way that workers get n / (p - 1) elemsents and master get n % (p - 1) elements
		 * By Master I mean it does collect the data and does all the pre & post sum processing
		 * Zeroth process is master
		 * Since elements are arr[i] = i, I have not created an explicit array to reduce the loop for creation
		 * */
	
		/* 
		* In all three methods I am assigning the last remaining elements to the master 
		*/
		for (int i = (p - 1) * nByp; i < n; i++) lSum = lSum + i;
		long sum = lSum;

		for (int i = 1; i < p; i++) {
			MPI_Recv(&lSum, 1, MPI_LONG, i, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			sum = sum + lSum;
		}
		cout << "Sum of all elements:" << endl << sum << endl;

	}
	else {
		switch (m) {
			case 1: {
				lSum = blockSum(r, nByp);
				break;
			}
			case 2: {
				//cout << r << " " << nByp << " " << p << endl;
				lSum = cyclicSum(r, nByp, p);
				break;
			}
			case 3: {
				lSum = blockCyclicSum(r, nByp, p);
				break;
			}
		}
		MPI_Send(&lSum, 1, MPI_LONG, 0, r, MPI_COMM_WORLD);
	}

	MPI_Finalize();
	return 0;
}

