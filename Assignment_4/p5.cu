// Write a CUDA program to compute the sum of an array of elements. Input:Number of elements in the array. Output: Array sum

// Error handler was copied from Dr. Rama's colab file shared to us on google classroom

#include<stdio.h>
#include<stdlib.h>
#include<time.h>


#define HANDLE_ERROR( err ) ( HandleError( err, __FILE__, __LINE__ ) )

__managed__ int n = 7;
__managed__ int sum = 0;


static void HandleError( cudaError_t err, const char *file, int line ) {
    if (err != cudaSuccess) {
        printf( "%s in %s at line %d\n", cudaGetErrorString(err), file, line);
        exit(EXIT_FAILURE);
    }
}

__global__ void sumItUp(int *arr) {
	int tid = threadIdx.x;
	if(tid < n) {
		atomicAdd(&sum, arr[tid]);
	}
}

int main() {
	
	scanf("%d", &n);
	srand(time(0));

	int *arr;
	int *c_arr;

	arr = (int *)malloc(n * sizeof(int));
	HANDLE_ERROR(cudaMalloc((void **)&c_arr, n * sizeof(int)));

	for (int i = 0; i < n; i++) {
		arr[i] = rand() % 1000;

		// To see the elements uncomment line 45 and 47, if this is 44
		// printf("%d ", arr[i]);
	}
	// puts(" ");

	HANDLE_ERROR(cudaMemcpy(c_arr, arr, n * sizeof(int), cudaMemcpyHostToDevice));

	sumItUp<<<1, n>>>(c_arr);
	cudaDeviceSynchronize();

	printf("%d\n", sum);

	free(arr);
	HANDLE_ERROR(cudaFree(c_arr));
	return 0;

}

		
	
	

