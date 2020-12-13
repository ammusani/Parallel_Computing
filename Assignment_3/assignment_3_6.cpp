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
	
    	int n;
    	cout << "Enter the number of elements in the array:" << endl;
	cin >> n;
    	long arr[n];
    	
    	myClock :: time_point end = myClock :: now();
	myClock :: duration d = end - start;
	unsigned int dur = d.count();
	minstd_rand0 gen(dur);
    	
    	for(int i = 0; i < n; i++) {
    		arr[i] = rand1(gen);
		/*
		 * Uncomment the following to see the values
    		cout << arr[i] << " ";
		*
		* */
    	}
    	cout << endl;
        
        long total_sum = 0;
    	#pragma omp parallel for reduction(+: total_sum)
        for(int i = 0; i < n; i++) {
            total_sum = total_sum + arr[i];
        }
    	cout << "Total sum:" << endl << total_sum << endl;
}
