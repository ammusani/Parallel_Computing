#include <bits/stdc++.h>
using namespace std;

int main () {
	int n;
	int s;
	int t;
	int l = 0;
	cout << "Number of threads, followed by start and then end" << endl;
	cin >> n >> s >> t;
	string fileName = "assignment_1.md";

	vector<vector<int>> A;

	ifstream graphFile;
	graphFile.open(fileName);

	/*
	 * Reading the graph from the file
	 * */

	for (int i = 0; i < n; i++) {
		string S;
		vector<int> v;
		graphFile >> S;
		for (int j = 0; j < S.size(); j++) v.push_back(S[j] - '0');
		A.push_back(v);
	}

	graphFile.close();
	
	vector<vector<int>> B = A;
	vector<vector<int>> C = A;

	if (A[s][t]) l = 1;
	else {
		for (l = 2; l < n; l++) {
			for (int i = 0; i < n; i++) {
				for (int j = 0; j < n; j++) {
					C[i][j] = 0;
					for (int k = 0; k < n; k++) C[i][j] += A[i][k] * B[k][j];
				}
			}
			A = C;
			if (A[s][t]) break;
		}
	}
	if (l < n) cout << "Shortest Path Length = " << l << endl;
	else cout << "No Shortest Path Exists" << endl;


	return 0;

}
