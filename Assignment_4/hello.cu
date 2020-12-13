#include<stdio.h>
#include<stdlib.h>

__global__ void print1() {
	printf("Hello! tid = %d bid = %d\n", threadIdx.x, blockIdx.x);
}

int main() {
	printf("Hello from Host!\n");
	print1<<<2, 3>>>();
	cudaDeviceSynchronize();
	return 0;
}

