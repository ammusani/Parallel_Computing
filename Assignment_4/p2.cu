// Write a CUDA program to compute the sum of two arrays. Input: Number of elements in the array. Output: Array of sums

// Used the Error Handler function written by Dr. Rama in his Colab shared to us on google classroom

#include<stdio.h>
#include<stdlib.h>
#include<time.h>


#define HANDLE_ERROR( err ) ( HandleError( err, __FILE__, __LINE__ ) )

__managed__ int n = 5;


static void HandleError( cudaError_t err, const char *file, int line ) {
    if (err != cudaSuccess) {
        printf( "%s in %s at line %d\n", cudaGetErrorString(err), file, line);
        exit(EXIT_FAILURE);
    }
}

__global__ void sumArrays(int *a, int *b, int *sum) {
	int tid = threadIdx.x;
	if(tid < n) {
		sum[tid] = b[tid] + a[tid];
	}
}

int main() {
	
	scanf("%d", &n);
	srand(time(0));

	int *a;
	int *b;
	int *sum;
	int *c_a;
	int *c_b;
	int *c_sum;

	a = (int *)malloc(n * sizeof(int));
	b = (int *)malloc(n * sizeof(int));
	sum = (int *)malloc(n * sizeof(int));

	HANDLE_ERROR(cudaMalloc((void **)&c_a, n * sizeof(int)));
	HANDLE_ERROR(cudaMalloc((void **)&c_b, n * sizeof(int)));
	HANDLE_ERROR(cudaMalloc((void **)&c_sum, n * sizeof(int)));

	for (int i = 0; i < n; i++) {
		a[i] = rand() % 1000;
		b[i] = rand() % 1000;

		// To see the elements uncomment line 54 and 56, if this is 53
		// printf("%d %d\n", a[i], b[i]);
	}
	// puts(" ");

	HANDLE_ERROR(cudaMemcpy(c_a, a, n * sizeof(int), cudaMemcpyHostToDevice));
	HANDLE_ERROR(cudaMemcpy(c_b, b, n * sizeof(int), cudaMemcpyHostToDevice));

	sumArrays<<<1, n>>>(c_a, c_b, c_sum);
	cudaDeviceSynchronize();


	HANDLE_ERROR(cudaMemcpy(sum, c_sum, n * sizeof(int), cudaMemcpyDeviceToHost));

	for (int i = 0; i < n; i++) {
		printf("sum[%d] = %d\n", i, sum[i]);
	}
	puts(" ");

	free(a);
	free(b);
	free(sum);
	HANDLE_ERROR(cudaFree(c_a));
	HANDLE_ERROR(cudaFree(c_b));
	HANDLE_ERROR(cudaFree(c_sum));

	return 0;

}

		
	
	

