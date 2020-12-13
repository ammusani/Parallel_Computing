#include <bits/stdc++.h>
#include <omp.h>

#define NUM_THREADS 6			// Assumed 6 cause I have a 6 core machine could be changed to your preference
#define BLOCK_SIZE 4			// Using Block Cyclic approach, change block size to your convinience

using namespace std;


int main (int argc, char *argv[]) {
	typedef chrono :: high_resolution_clock myClock;
	myClock :: time_point start = myClock :: now();

	int n = atoi(argv[1]);
	double t = atof(argv[2]);
	double *arr = new double[(n + 2) * (n + 2)];
	double *old_arr = new double[(n + 2) * (n + 2)];
	memset(old_arr, 0, (n + 2) * (n + 2) * sizeof(double));
	double localMaxD;
	double localD;

	myClock :: time_point stop = myClock :: now();
	myClock :: duration d = stop - start;
	
	omp_set_num_threads(NUM_THREADS);

	minstd_rand gen1(d.count());
	default_random_engine re;
	uniform_real_distribution<double> gen(0, 255);
	
	int skip_size = NUM_THREADS * BLOCK_SIZE;

	/*
	 * Initializing matrix with random values
	 * */ 

	#pragma omp parallel for collapse(2) shared(arr)
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
	int flag = 1;
	start = myClock :: now();	
	#pragma omp parallel shared(arr, old_arr, n, flag, iter, skip_size) private(localMaxD, localD) 
	{
		int tid = omp_get_thread_num();
		int numT = omp_get_num_threads();
		vector<int> k;
		for (int i = tid * BLOCK_SIZE + n + 2; i < (n + 2) * (n + 1); i += skip_size) {
			for (int j = 0; j < BLOCK_SIZE; j++) {
				int l = (i + j) % (n + 2);
				if (i + j >= (n + 2) * (n + 1)) break;
				if (l && l != n + 1) k.push_back(i + j);
			}
		}

		while (flag) {
		
			localMaxD = 0;
			localD = 0;
			for (int i = 0; i < k.size(); i++) old_arr[k[i]] = arr[k[i]];
			#pragma omp barrier
			
			if (!tid) {
				flag = 0;
				++iter;
			}
			#pragma omp barrier
			/*i
			if (!tid) for (int i = 0; i < n + 2; i++) {
				for (int j = 0; j < n + 2; j++) cout << old_arr[i * (n + 2) + j] << " ";
				cout << endl;
			}
			*/

			for (int i = 0; i < k.size(); i++) {
				int m = k[i];
				arr[m] = (old_arr[m] + old_arr[m - 1] + old_arr[m + 1] + old_arr[m + n + 2] +old_arr[m - n - 2]) / 5;
				localD = abs(arr[m] - old_arr[m]);
				if (localMaxD < localD) localMaxD = localD;
			}

			if(localMaxD > t) {
				#pragma omp atomic
					++flag;
			}
			
			#pragma omp barrier
		}

	}
	stop = myClock :: now();
	d = chrono :: duration_cast<chrono :: nanoseconds>(stop - start);
	cout << "Time = " << d.count() << endl;
	cout << "Iterations = " << iter << endl;
	cout << "Number of Threads = " << NUM_THREADS << endl;

	return 0;
}

