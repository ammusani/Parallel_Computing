#include <bits/stdc++.h>
#include <mpi.h>
#include <time.h>

using namespace std;

int main (int argc, char **argv) {
	
	/*
	 * Variable Declaration
	 * */

	int n;			// Dim of adjacency matrix
	int s;			// Start of the path
	int t;			// End of the path
	int p;			// Number of Processes
	int r;			// Rank of the process
	int l = 0;		// length of shortes path
	string fileName = "assignment_1.md";

	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &p);
	MPI_Comm_rank(MPI_COMM_WORLD, &r);

	/*
	 * Initializing the graph and different variables
	 * */
	if(r == 0) {
		
		/* Zero Indexing Assumed
		 * */

       		cout << "Enter the Dim of the adjacency matrix:" << endl;
       		cin >> n;

       		cout << "Enter the start point of the path:" << endl;
       		cin >> s;

       		cout << "Enter the end point of the path:" << endl;
       		cin >> t;
       		cout << endl;
	}

	/*
	 * Broadcasting the n, s, values to all the thread
	 * */

	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&s, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&t, 1, MPI_INT, 0, MPI_COMM_WORLD);

	int A[n][n];
	int B[n][n];
	int C[n][n];

	if(r == 0) {
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

		/*
		 * Checking the path length if equal to or not
		 * */

		if (A[s][t] == 1) l = 1;
		
	}
	
	MPI_Bcast(&l, 1, MPI_INT, 0, MPI_COMM_WORLD);

	int m = n / p;
	int flag = 0;

	if (l != 1) {
	
		MPI_Bcast(&A, n * n, MPI_INT, 0, MPI_COMM_WORLD);
		memcpy(&B, &A, sizeof(A));
		

		/*
		 * For loop to check for all the path lengths from 2 to n - 1
		 * */

		for (l = 2; l < n; l++) {


			clock_t start = clock();
			
			
			for (int i = 0; i < n; i++) {

				/* Diving the work according to the thread number */

				if (r == i / m) {
					for (int j = 0; j < n; j++) {
						C[i][j] = 0;
						for (int k = 0; k < n; k++) C[i][j] += A[i][k] * B[k][j];
					}

					/*
					 * Updating the ith row and not repolacing for all cause it can be a local value
					 * */
					for (int j = 0; j < n; j++) {
						A[i][j] = C[i][j];
					}

				}
				
			}
			
			if (r == s / m) {
				flag = A[s][t];
			}
			MPI_Bcast(&flag, 1, MPI_INT, s / m, MPI_COMM_WORLD);

			if (r == 0) {
				clock_t stop = clock();
				cout << "Execution Time(in mS) for " << l << " lenght step = " << (double)(stop - start) * 1000/ CLOCKS_PER_SEC << endl;
			}

			if (flag) break;
		}
	}

	MPI_Barrier(MPI_COMM_WORLD);

	if (r == 0) {
		if (l < n) cout << "Shortest Path Length = " << l << endl;
		else cout << "No Shortest Path Exists" << endl;
	}

	MPI_Finalize();	
	
	return 0;
}
