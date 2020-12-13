#include <bits/stdc++.h>  
#define p 14 

using namespace std;

long sum[p] = {0};
int nByp;

long rand1 (minstd_rand0 &gen) {
	/*
	 * Will return a number from 0 to 255
	 * */
	 
	long long r1 = gen();
	long long r2 = gen();
	return (abs(r1 * r2) % 256);
}

void array_sum(int tid, vector<long> &arr) { 
	for (int j = tid * nByp; j < (tid + 1) * nByp; j++) 
		sum[tid] = sum[tid] + arr[j];
		
	pthread_exit(NULL);
} 

int main() 
{ 
	/*
	 * Random Number Generator Initialization
	 * */
	typedef chrono :: high_resolution_clock myClock;
	myClock :: time_point start = myClock :: now();
	
	int n;
	cout << "Enter the number of elements in the array:" << endl;
	cin >> n;
	vector<long> arr;
	vector<thread> threads;
	nByp = n / (p - 1);
	
	myClock :: time_point end = myClock :: now();
	myClock :: duration d = end - start;
	unsigned int dur = d.count();
	minstd_rand0 gen(dur);
		
	for (int i = 0; i < n; i++) {
		arr.push_back(rand1(gen));
		/*
		 * Uncomment the following to see the values
		cout << arr[i] << " ";
		*
		* */
	}
	cout << endl;
	
	/* 
	 * Creating p threads
	 * */ 
	for (int i = 0; i < p; i++) {
		threads.push_back(thread(array_sum, i, ref(arr)));
	} 

	/*
	 * joining p threads
	 * */
	for (auto &i : threads) {
		i.join();
	} 

	
	/* 
	 * adding sum of all threads
	 * */ 
	long total_sum = 0; 
	for (int i = 0; i < p; i++) 
		total_sum += sum[i]; 

	cout << "Total sum:" << endl << total_sum << endl; 

	return 0; 
} 

