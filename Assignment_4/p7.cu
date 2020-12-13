// Use Thrust library to sort a vector of elements.

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include<thrust/host_vector.h>
#include<thrust/device_vector.h>
#include<thrust/sort.h>
#include<thrust/copy.h>

int main() {
	long n = pow(10, 7);
	srand(time(0));
	
	thrust :: host_vector<long> h(n);
	for (int i = 0; i < n; i++) h[i] = rand() % (8 * n);
	thrust :: device_vector<long> d = h;
	thrust :: sort(d.begin(), d.end());
	thrust :: copy(d.begin(), d.end(), h.begin());
	
	for (int i = 0; i < n; i++) {
		printf("%ld ", h[i]);
		if (!(i % 5)) puts("");
	}
	puts("");
	return 0;
}
