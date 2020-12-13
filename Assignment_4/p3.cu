// Write a CUDA program to multiply two matrices. Input: Matrix 1 size: m X n Matrix 2 size: n X p. Output: Result matrix


// Used the Error Handler function written by Dr. Rama in his Colab shared to us on google classroom

#include<stdio.h>
#include<stdlib.h>
#include<time.h>


#define HANDLE_ERROR( err ) ( HandleError( err, __FILE__, __LINE__ ) )

__managed__ int m = 5;
__managed__ int n = 5;
__managed__ int p = 5;

int GCD(int a, int b) {
	if (!b) return a;
	return GCD(b, a % b);
}

static void HandleError( cudaError_t err, const char *file, int line ) {
    if (err != cudaSuccess) {
        printf( "%s in %s at line %d\n", cudaGetErrorString(err), file, line);
        exit(EXIT_FAILURE);
    }
}

__global__ void matrixMult(int *a, int *b, int *mul) {
	int row = blockIdx.x * blockDim.x + threadIdx.x;
	int col = blockIdx.y * blockDim.y + threadIdx.y;

	if(row < m && col < p) {
		int t = row * p + col;
		mul[t] = 0;
		for (int i = 0; i < n; i++) mul[t] += a[row * n + i] * b[i * p + col];
	}
}

int main() {
	
	scanf("%d %d %d", &m, &n, &p);
	puts(" ");
	srand(time(0));

	int *a;
	int *b;
	int *mul;
	int *c_a;
	int *c_b;
	int *c_mul;

	a = (int *)malloc(m * n * sizeof(int));
	b = (int *)malloc(n * p * sizeof(int));
	mul = (int *)malloc(m * p * sizeof(int));

	HANDLE_ERROR(cudaMalloc((void **)&c_a, m * n * sizeof(int)));
	HANDLE_ERROR(cudaMalloc((void **)&c_b, n * p * sizeof(int)));
	HANDLE_ERROR(cudaMalloc((void **)&c_mul, m * p * sizeof(int)));

	for (int i = 0; i < m; i++) {
		for(int j = 0; j < n; j++) {
			a[i * n + j] = rand() % 100;

		// To see the elements of a uncomment line 66, 68  and 70, if this is 65
		// printf("%d ", a[i * n + j]);
		}
		// puts(" ");
	}
	// puts(" ");
	
	for (int i = 0; i < n; i++) {
		for(int j = 0; j < p; j++) {
			b[i * p + j] = rand() % 100;

		// To see the elements of b uncomment line 77, 79 and 81, if this is 76
		// printf("%d ", b[i * p + j]);
		}
		// puts(" ");
	}
	// puts(" ");

	HANDLE_ERROR(cudaMemcpy(c_a, a, m * n * sizeof(int), cudaMemcpyHostToDevice));
	HANDLE_ERROR(cudaMemcpy(c_b, b, n * p * sizeof(int), cudaMemcpyHostToDevice));

	int gcd = GCD(m, p);

	dim3 threads(m / gcd, p / gcd);
	dim3 blocks(gcd, gcd);

	matrixMult<<<blocks, threads>>>(c_a, c_b, c_mul);
	cudaDeviceSynchronize();


	HANDLE_ERROR(cudaMemcpy(mul, c_mul, m * p * sizeof(int), cudaMemcpyDeviceToHost));

	for (int i = 0; i < m; i++) {
		for(int j = 0; j < p; j++) printf("%d ", mul[i * p + j]);
		puts(" ");
	}
	puts(" ");

	free(a);
	free(b);
	free(mul);
	HANDLE_ERROR(cudaFree(c_a));
	HANDLE_ERROR(cudaFree(c_b));
	HANDLE_ERROR(cudaFree(c_mul));

	return 0;

}

		
	
	

