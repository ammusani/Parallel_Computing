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
	int r;			// Rank of the process
	string fileName = "assignment_1.md";

	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &p);
	MPI_Comm_rank(MPI_COMM_WORLD, &r);
	sqrtP = int(sqrt(p));

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

	MPI_Finalize();	
	
	return 0;
}
