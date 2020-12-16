// Implement BFS on CUDA.
// The graph is not weighted but it is directed. The BFS algorithms is same just have to change the graph to unwieghted.

// Error handler was copied from Dr. Rama's colab file shared to us on google classroom

#include<stdio.h>
#include<stdlib.h>
#include<time.h>


#define HANDLE_ERROR( err ) ( HandleError( err, __FILE__, __LINE__ ) )

__managed__ int n;

static void HandleError( cudaError_t err, const char *file, int line ) {
    if (err != cudaSuccess) {
        printf( "%s in %s at line %d\n", cudaGetErrorString(err), file, line);
        exit(EXIT_FAILURE);
    }
}

__global__ void BFS_Kernel(int *Va, int *Ea, int *Fa, int *Xa, int *Ca) {
	int tid = threadIdx.x;
	if (tid < n) {
		if(Fa[tid]) {
			Fa[tid] = 0;
			Xa[tid] = 1;
			
			for (int i = Va[tid]; i < Va[tid + 1]; i++) {
				int j = Ea[i];

				if(!Xa[j]) {
					Ca[j] = Ca[tid] + 1;
					Fa[j] = 1;
				}
			}
		}
	}
}

int main() {
	
	srand(time(0));	
	n = 50;
	int src = rand() % n;
	int limit = 5;		// Limit on maximum number of edges from a vertex

	printf("Number of Vertices = %d\nStarting Vertex = %d\n", n, src);

	int *Va;
	int *c_Va;

	int *Ea;
	int *c_Ea;

	int end = 0;	
	Va = (int *)malloc((n + 1) * sizeof(int));
	HANDLE_ERROR(cudaMalloc((void **)&c_Va, (n + 1) * sizeof(int)));
	for (int i =0; i < n; i++) {
		Va[i] = end;
		end = end + (rand() % limit);
	}
	Va[n] = end;
	HANDLE_ERROR(cudaMemcpy(c_Va, Va, (n + 1) * sizeof(int), cudaMemcpyHostToDevice));

	Ea = (int *)malloc(end * sizeof(int));
	HANDLE_ERROR(cudaMalloc((void **)&c_Ea, end * sizeof(int)));
	for (int i = 0; i < end; i++) {
		Ea[i] = (rand()) % n;
	}
	HANDLE_ERROR(cudaMemcpy(c_Ea, Ea, end * sizeof(int), cudaMemcpyHostToDevice));

	/*
	   Uncomment this to see the graph
 
	for (int i = 0; i < n; i++) printf("%d ", Va[i]);
	puts(" ");
	for (int i = 0; i < end; i++) printf("%d ", Ea[i]);
	puts(" ");
	
	/**/

	int *T;
	T = (int *)malloc(n * sizeof(n));
	

	int *c_Fa;
	HANDLE_ERROR(cudaMalloc((void **)&c_Fa, n * sizeof(int)));
	memset(T, 0, n * sizeof(int));
	T[src] = 1;
	HANDLE_ERROR(cudaMemcpy(c_Fa, T, n * sizeof(int), cudaMemcpyHostToDevice));

	int *c_Xa;
	HANDLE_ERROR(cudaMalloc((void **)&c_Xa, n * sizeof(int)));
	memset(T, 0, n * sizeof(int));
	HANDLE_ERROR(cudaMemcpy(c_Xa, T, n * sizeof(int), cudaMemcpyHostToDevice));

	int *c_Ca;
	HANDLE_ERROR(cudaMalloc((void **)&c_Ca, n * sizeof(int)));
	memset(T, -1, n * sizeof(int));
	T[src] = 0;
	HANDLE_ERROR(cudaMemcpy(c_Ca, T, n * sizeof(int), cudaMemcpyHostToDevice));

	int flag = 1;

	do {
		flag = 0;
		
		BFS_Kernel <<<1, n>>> (c_Va, c_Ea, c_Fa, c_Xa, c_Ca);

		cudaDeviceSynchronize();
		HANDLE_ERROR(cudaMemcpy(T, c_Fa, n * sizeof(int), cudaMemcpyDeviceToHost));
		
		for (int i = 0; i < n; i++) {
			if (T[i]) {
				flag = 1;
				break;
			}
		}

		
	} while(flag);

	HANDLE_ERROR(cudaMemcpy(T, c_Ca, n * sizeof(int), cudaMemcpyDeviceToHost));
	for (int i = 0; i < n; i++)
		printf("cost to reach %dth node = %d\n", i, T[i]);

	printf("\nNote: -1 means you can not reach the node from the current starting node\n");

	free(Va);
	free(Ea);
	free(T);
	cudaFree(c_Va);
	cudaFree(c_Ea);
	cudaFree(c_Fa);
	cudaFree(c_Xa);
	cudaFree(c_Ca);

	return 0;

}
