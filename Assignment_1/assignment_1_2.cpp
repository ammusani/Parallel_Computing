/*
 * Problem: A -> Adjacency Matrix of an Undirected, Unit Weight Graph
 * To find the Shortest distance between s, t.
 *
 * Shortest Distance x, where A^x[s][t] != 0
 *
 * The problem is solved by diving the whole square-matrix of Dim n into p square-submatrices of dim n / sqrt(p)
 * Where p is number of threads
 * The idea is to not have the whole data but acquire it from another process when required
 * */


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

	/*
	 * Initializing the submatrices from the file
	 * */


	ifstream graphFile;
	graphFile.open(fileName);
	
	for (int i = 0; i < n; i++) {
		string S;
		graphFile >> S;

		for (int j = 0; j < n; j++) if (rank == (i / k) * sqrtP + (j / k)) A[i % k][j % k] = S[j] - '0';

	}

	graphFile.close();

	
	/*
	 * Check if edge is already present between s & t
	 * */

	if (rank == (s / k) * sqrtP + (t / k) && A[s % k][t % k]) l = 1;
	MPI_Bcast(&l, 1, MPI_INT, (s / k) * sqrtP + (t / k), MPI_COMM_WORLD);

	/*
	 * If the edge is not present then find the path length
	 * */

	if (l != 1) {

		int flag = 0;

		memcpy(&B, &A, sizeof(B));
		memcpy(&Bo, &B, sizeof(B));

		/*
		 * The Matrix is divided into a matrix of submatrices
		 * Calculating the indices of each submatrix
		 * */

		int row = floor(rank / sqrtP);
		int col = (int)rank % sqrtP;


		/*
		 * Doing at max n - 2 multiplication to check if an edge exists
		 * */

		for (l = 2; l < n; l++) {

			/*
			 * Starting the multiplication
			 * */

		 	double start = MPI_Wtime();


			/* 
			 * Rotating ith row of submatrices by i times
			 * */

			if (row > 0) {
				int left = (rank - row) % sqrtP + row * sqrtP;
				int right = (rank + row) % sqrtP + row * sqrtP;
				MPI_Send(&A, k * k, MPI_INT, left, 0, MPI_COMM_WORLD);
				MPI_Recv(&A, k * k, MPI_INT, right, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			}

			/*
			 * Rotating jth column of submatices by j times
			 * */

			if (col > 0) { 
				int up = (rank - col * sqrtP + p) % p;
				int down = (rank + col * sqrtP) % p;
				MPI_Send(&B, k * k, MPI_INT, up, 0, MPI_COMM_WORLD);
				MPI_Recv(&B, k * k, MPI_INT, down, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			}


			memset(C, 0, sizeof(C));  // Making sure C is initialized to zero


			/*
			 * Since there are sqrt(p) rows and columns, that many multiplications are required
			 * */
			for (int i = 0; i < sqrtP; i++) {
				
				/* 
				 * Submatrix multiplication with the available data
				 * */

				for (int i1 = 0; i1 < k; i1++) {
					for (int i2 = 0; i2 < k; i2++) {
						for (int i3 = 0; i3 < k; i3++) {
							C[i1][i2] += A[i1][i3] * B[i3][i2];
						}
					}
				}


				/*
				 * Rotating each row by 1 to left
				 * */

				int left = (rank - 1 + sqrtP) % sqrtP + row * sqrtP;
				int right = (rank + 1) % sqrtP + row * sqrtP;
				MPI_Send(&A, k * k, MPI_INT, left, 0, MPI_COMM_WORLD);
				MPI_Recv(&A, k * k, MPI_INT, right, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);


				/*
				 * Rotating each column by 1 to up
				 * */

				int up = (rank - sqrtP + p) % p;
				int down = (rank + sqrtP) % p;
				MPI_Send(&B, k * k, MPI_INT, up, 0, MPI_COMM_WORLD);
				MPI_Recv(&B, k * k, MPI_INT, down, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	
			}
			
			/*
			 * Matrix Multiplication ends her
			 * */
			double stop = MPI_Wtime();

			/*
			 * Storing C in A for next iteration
			 * And restoring B to the original value
			 * Since the goal is to find A^k, where k < n & k is the shortest path length
			 * */
			memcpy(&A, &C, sizeof(A));
		       	memcpy(&B, &Bo, sizeof(B));	
			

			/*
			 * Checking for edge b/w s & t
			 * */

			if (rank == (s / k) * sqrtP + (t / k) && A[s % k][t % k]) flag = 1;
			MPI_Bcast(&flag, 1, MPI_INT, (s / k) * sqrtP + (t / k), MPI_COMM_WORLD);

			/*
			 * Printing multiplication time for ith step
			 * */
			if (rank == 0) cout << "Execution Time(in ms) for " << l << " length step = " << stop - start << endl;

			/*
			 * Breaking if edge found
			 * */
			//MPI_Barrier(MPI_COMM_WORLD);
			if(flag) break;

		} 


	}

	/*
	 * Printing Path Value
	 * */
	cout << endl;
	if (rank == 0) {
		if (l < n) cout << "Shortest Path Length = " << l << endl;
		else cout << "No Shortest Path Exists" << endl;
	}



	MPI_Finalize();	
	
	return 0;
}
