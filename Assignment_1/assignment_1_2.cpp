#include <bits/stdc++.h>
#include <mpi.h>
using namespace std;

int main (int argc, char **argv) {
	
	/*
	 * Variable Declaration
	 * */

	int n;			// Dim of adjacency matrix
	int s;			// Start of the path
	int t;			// End of the path
	int p;			// Number of Processes
	int sqrtP;
	int rank;		// Rank of the processi
	int k;			// n / sqrt(p)
	int l = 0;
	string fileName = "assignment_1.md";

	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &p);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	sqrtP = int(sqrt(p));

	/*
	 * Initializing the graph and different variables
	 * */
	if(rank == 0) {
		
		/* Zero Indexing Assumed
		 * */

       		cout << "Enter the Dim of the adjacency matrix:" << endl;
       		cin >> n;

       		cout << "Enter the start point of the path:" << endl;
       		cin >> s;

       		cout << "Enter the end point of the path:" << endl;
       		cin >> t;
       		cout << endl;
	
		int A[n][n];

       		cout << "Generating the adjacency matrix\nIt will be stored in " << fileName << endl;
	
		srand(time(0));

		/* Creating a random adjacency matrix
		 * Assumption 1: There's no self edge 
		 * Taking time(0) as the random seed 
		 * */
	
		for (int i = 0; i < n; i++) A[i][i] = 0;
		
		for (int i = 0; i < n; i++) {
			for (int j = i + 1; j < n; j++) {
				A[i][j] = rand() % 2;
			       	A[j][i] = A[i][j];
			}
		}


		/* Storing the graph in the file
	 	* */

		ofstream graphFile;
		graphFile.open(fileName);

		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				graphFile << A[i][j];
			
			}
			graphFile << endl;
		
		}
		graphFile.close();
		
	}



	/*
	 * Broadcasting the n, s, t, values to all other threads from thread 0
	 * */

	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&s, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&t, 1, MPI_INT, 0, MPI_COMM_WORLD);

	k = n / sqrtP;

	int A[k][k];
	int B[k][k];
	int C[k][k];
	int Bo[k][k];

	ifstream graphFile;
	graphFile.open(fileName);
	
	for (int i = 0; i < n; i++) {
		string S;
		graphFile >> S;

		for (int j = 0; j < n; j++) if (rank == (i / k) * sqrtP + (j / k)) A[i % k][j % k] = S[j] - '0';

	}

	graphFile.close();

	if (rank == (s / k) * sqrtP + (t / k) && A[s % k][t % k]) l = 1;

	if (l != 1) {

		int flag = 0;
		memcpy(&B, &A, sizeof(B));
		memcpy(&Bo, &B, sizeof(B));

		for (l = 2; l < n; l++) {
		 	double start = MPI_Wtime();
			for (int i = 1; i < sqrtP; i++) {

				if (rank / sqrtP == i) {
					int left = (rank - i + sqrtP) % sqrtP + i * sqrtP;
					int right = (rank + i) % sqrtP + i * sqrtP;
					MPI_Send(&A, k * k, MPI_INT, left, rank, MPI_COMM_WORLD);
					MPI_Recv(&A, k * k, MPI_INT, right, right, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

				}

				if (rank % sqrtP == i) { 
					int up = (rank - i * sqrtP + p) % p;
					int down = (rank + i * sqrtP) % p;
					MPI_Send(&B, k * k, MPI_INT, up, rank , MPI_COMM_WORLD);
					MPI_Recv(&B, k * k, MPI_INT, down, down, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				}

			}
			memset(C, 0, sizeof(C));

			for (int q = 0; q < sqrtP; q++) {
				for (int i = 0; i < sqrtP; i++) {
					for (int i1 = 0; i1 < sqrtP; i1++)
						for (int i2 = 0; i2 < sqrtP; i2++)
							for (int i3 = 0; i3 < sqrtP; i3++) C[i1][i2] += A[i1][i3] * B[i3][i2];

					int left = (rank - 1 + sqrtP) % sqrtP + i * sqrtP;
					int right = (rank + 1 + sqrtP) % sqrtP + i * sqrtP;

					int up = (rank - sqrtP + p) % p;
					int down = (rank + sqrtP) % p;

					MPI_Send(&A, k * k, MPI_INT, left, rank, MPI_COMM_WORLD);
					MPI_Recv(&A, k * k, MPI_INT, right, right, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

					MPI_Send(&B, k * k, MPI_INT, up, rank, MPI_COMM_WORLD);
					MPI_Recv(&B, k * k, MPI_INT, down, down, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				}

			}
			
			double stop = MPI_Wtime();

			memcpy(&A, &C, sizeof(A));
		       	memcpy(&B, &Bo, sizeof(B));	
			
			if (rank == (s / k) * sqrtP + (t / k) && A[s % k][t % k]) flag = 1;
			MPI_Bcast(&flag, 1, MPI_INT, (s / k) * sqrtP + (t / k), MPI_COMM_WORLD);
			if (rank == 0) cout << "Execution Time(in ms) for " << l << " length step = " << stop - start << endl;

			if(flag) break;

		} 


	}
	if (rank == 0) {
		if (l < n) cout << "Shortest Path Length = " << l << endl;
		else cout << "No Shortest Path Exists" << endl;
	}



	MPI_Finalize();	
	
	return 0;
}
