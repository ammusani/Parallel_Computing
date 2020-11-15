#include<bits/stdc++.h>
#include<omp.h>
using namespace std;

long rand1 (minstd_rand0 &gen) {
	/*
	 * Will return a number from 0 to 255
	 * */
	 
	long long r1 = gen();
	long long r2 = gen();
	return (abs(r1 * r2) % 256);
}

int main(int argc,char** argv) {

	/*
	 * Random Number Generator Initialization
	 * */
	typedef chrono :: high_resolution_clock myClock;
	myClock :: time_point start = myClock :: now();
	
    	int m;
	int n;
	int p;

    	cout << "Enter m, n & p:" << endl;
	cin >> m >> n >> p;
    	long a[m][n];
	long b[n][p];
    	
    	myClock :: time_point end = myClock :: now();
	myClock :: duration d = end - start;
	unsigned int dur = d.count();
	minstd_rand0 gen(dur);
    	
    	for(int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) a[i][j] = rand1(gen);
    	}
        for(int i = 0; i < n; i++) {
		for (int j = 0; j < p; j++) b[i][j] = rand1(gen);
    	}

	cout << endl;
	/*
	 * Uncomment the following to print both the matrix
	 for(int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) cout << a[i][j] << " ";
		cout << endl;
    	}
	cout << endl;

        for(int i = 0; i < n; i++) {
		for (int j = 0; j < p; j++) cout << b[i][j] << " ";
		cout << endl;
    	}
	cout << endl;
	 /**
	 * */

        long c[m][p];
    	#pragma omp parallel for collapse(2)
        for(int i = 0; i < m; i++) {
            for(int j = 0; j < p; j++) {
		    c[i][j] = 0;
		    for (int k = 0; k < n; k++) c[i][j] = c[i][j] + (a[i][k] * b[k][j]);
	    }
        }

	for(int i = 0; i < m; i++) {
		for (int j = 0; j < p; j++) cout << c[i][j] << " ";
		cout << endl;
    	}
	cout << endl;
}
