#include <bits/stdc++.h>

using namespace std;


int index(int i, int j, int n) {
	return i * (n + 2) + j; 
}

int main (int argc, char *argv[]) {
	typedef chrono :: high_resolution_clock myClock;
	myClock :: time_point start = myClock :: now();

	int n = atoi(argv[1]);
	double t = atof(argv[2]);
	double *arr = new double[(n + 2) * (n + 2)];
	double *old_arr = new double[(n + 2) * (n + 2)];
	double maxD = numeric_limits<double> :: max();
	double localMaxD;
	double localD;

	myClock :: time_point stop = myClock :: now();
	myClock :: duration d = stop - start;

	minstd_rand gen1(d.count());
	default_random_engine re;
	uniform_real_distribution<double> gen(0, 255);

	/*
	 * Initializing matrix with random values
	 * */ 
	
	for (int i = 1; i < n + 1; i++) for (int j = 1; j < n + 1; j++) arr[i * (n + 2) + j] = ((double) gen1() * gen1() / (1000000007 * gen1())) * gen(re);		// A typical random number genrator, could be changed to anything

	for (int i = 0; i < n + 2; i++) {
		if (i == 0 || i == n + 1) for (int j = 0; j < n + 2; j++) arr[i * (n + 2) + j] = 0;
		else {
			arr[i * (n + 2)] = 0;
			arr[i * (n + 2) + n + 1] = 0;
		}
	}


	/*
	 * un comment the following to print the original matrix
	 *
	 * 
 	
	for (int i = 1; i < n + 1; i++) {
		for (int j = 1; j < n + 1; j++) cout << arr[i * (n + 2) + j] << " ";
		cout << endl;
	}
	/* 
	 * */

	long iter = 0;
	int rem = 1;				// Give the value after which you want to see the matrix
	start = myClock :: now();
	while (maxD > t) {
		memcpy(old_arr, arr, sizeof(double) * (n + 2) * (n + 2));
		localMaxD = 0;
		for (int i = 1; i < n + 1; i++) {
			for (int j = 1; j < n + 1; j++) {
				int k = i * (n + 2) + j;
				arr[k] = (old_arr[k] + old_arr[k - 1] + old_arr[k + 1] + old_arr[k - n - 2] + old_arr[k + n + 2]) / 5;
				localD = abs(arr[k] - old_arr[k]);
				if (localMaxD < localD) localMaxD = localD;

			}
		}
		maxD = localMaxD;

		/*
		 * un comment the following to print the matrix every mth iteration
	 	*
	 	*

		if(!(iter % rem))for (int i = 1; i < n + 1; i++) {
			for (int j = 1; j < n + 1; j++) cout << arr[i * (n + 2) + j] << " ";
			cout << endl;
		}

		/*
		 * */

		iter++;		
	}
	stop = myClock :: now();
	d = stop - start;
	cout << d.count() << endl;
	cout << iter << endl;


	return 0;
}

