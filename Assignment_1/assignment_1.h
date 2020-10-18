#include <bits/stdc++.h>
#include <mpi.h>
using namespace std;

vector<int> string_to_vector(string s) {
	stringstream ss(s);

	int num;
	vector<int> v;

	while(ss >> num) v.push_back(num);

	cout << s << endl;

	return v;
}

vector<int> getRow (int a, string S) {
	
	ifstream graphFile;
	graphFile.open(S);

	string s;
	for (int i = 0; i < a; i++) getline(graphFile, s);

	graphFile.close();

	return string_to_vector(s);

}



int getRank (int n, int s, int t, string fileName, int argc, char *argv[]) {
	vector<vector<int>> A(n, vector<int> (n, 0));
	vector<vector<int>> B(n, vector<int> (n, 0));
	vector<vector<int>> C(n, vector<int> (n, 0));

	ifstream graphFile;
	graphFile.open(fileName);

	for (int i = 0; i < n; i++) {
		string S;
		getline(graphFile, S);
		A.push_back(string_to_vector(S));

	}	
	graphFile.close();

	B = A;

	if(A[s][t]) cout << "Shortest Path Length = 1" << endl;
	else {

		for (int i = 0; i < n; i++) {
		
			matrixMatrixMult(A, B, C, argc, argv);

			if (C[s][t]) return i;
			A = C;
		}
	}

	return n;
}

