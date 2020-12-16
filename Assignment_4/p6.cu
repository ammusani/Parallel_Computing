// Implement SSSP on CUDA.
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

__global__ void SSSP_Kernel1(int *Va, int *Ea, int *Wa, int *Ma, int *Ca, int *Ua) {
	int tid = threadIdx.x;
	if (tid < n) {
		if (Ma[tid]) {
			Ma[tid] = 0;
		
			for (int i = Va[tid]; i < Va[tid + 1]; i++) {
				int j = Ea[i];
				int k = Ca[tid] + Wa[j];
				if (Ua[j] > k) Ua[j] = k;
			}
		}
	}
}

__global__ void SSSP_Kernel2 (int *Va, int *Ea, int *Wa, int *Ma, int *Ca, int *Ua) {
	
	int tid = threadIdx.x;
	if (tid < n) {
		if(Ca[tid] > Ua[tid]) {
			Ca[tid] = Ua[tid];
			Ma[tid] = 1;
		}
		Ua[tid] = Ca[tid];
	}
}

int main() {
	
	srand(time(0));	
	n = 50;
	int src = rand() % n;
	int limit1 = 5;		// Limit on maximum number of edges from a vertex
	int limit2 = 500;	// Limit on weights

	printf("Number of Vertices = %d\nStarting Vertex = %d\n", n, src);

	int *Va;
	int *c_Va;

	int *Ea;
	int *c_Ea;

	int *Wa;
	int *c_Wa;

	int end = 0;	
	Va = (int *)malloc((n + 1) * sizeof(int));
	HANDLE_ERROR(cudaMalloc((void **)&c_Va, (n + 1) * sizeof(int)));
	for (int i =0; i < n; i++) {
		Va[i] = end;
		end = end + (rand() % limit1);
	}
	Va[n] = end;
	HANDLE_ERROR(cudaMemcpy(c_Va, Va, (n + 1) * sizeof(int), cudaMemcpyHostToDevice));

	Ea = (int *)malloc(end * sizeof(int));
	Wa = (int *)malloc(end * sizeof(int));
	int tWeight = 0;
	HANDLE_ERROR(cudaMalloc((void **)&c_Ea, end * sizeof(int)));
	HANDLE_ERROR(cudaMalloc((void **)&c_Wa, end * sizeof(int)));
	for (int i = 0; i < end; i++) {
		Ea[i] = (rand()) % n;
		Wa[i] = (rand()) % limit2;
		tWeight = tWeight + Wa[i];
	}
	tWeight = tWeight * 10;
	HANDLE_ERROR(cudaMemcpy(c_Ea, Ea, end * sizeof(int), cudaMemcpyHostToDevice));
	HANDLE_ERROR(cudaMemcpy(c_Wa, Wa, end * sizeof(int), cudaMemcpyHostToDevice));

	/*
	   Uncomment this to see the graph
	
	for (int i = 0; i < n; i++) printf("%d ", Va[i]);
	puts(" ");
	for (int i = 0; i < end; i++) printf("%d ", Ea[i]);
	puts(" ");
	for (int i = 0; i < end; i++) printf("%d ", Wa[i]);
	puts(" ");	
	/**/

	int *T;
	T = (int *) malloc(n * sizeof(n));
	

	int *c_Ma;
	HANDLE_ERROR(cudaMalloc((void **)&c_Ma, n * sizeof(int)));
	memset(T, 0, n * sizeof(int));
	T[src] = 1;
	HANDLE_ERROR(cudaMemcpy(c_Ma, T, n * sizeof(int), cudaMemcpyHostToDevice));

	int *c_Ua;
	HANDLE_ERROR(cudaMalloc((void **)&c_Ua, n * sizeof(int)));
	for (int i = 0; i < n; i++) T[i] = tWeight;
	T[src] = 0;
	HANDLE_ERROR(cudaMemcpy(c_Ua, T, n * sizeof(int), cudaMemcpyHostToDevice));

	int *c_Ca;
	HANDLE_ERROR(cudaMalloc((void **)&c_Ca, n * sizeof(int)));
	HANDLE_ERROR(cudaMemcpy(c_Ca, T, n * sizeof(int), cudaMemcpyHostToDevice));

	int flag = 1;

	do {
		flag = 0;
		
		SSSP_Kernel1 <<<1, n>>> (c_Va, c_Ea, c_Wa, c_Ma, c_Ca, c_Ua);
		cudaDeviceSynchronize();

		SSSP_Kernel2 <<<1, n>>> (c_Va, c_Ea, c_Wa, c_Ma, c_Ca, c_Ua);
		cudaDeviceSynchronize();

		HANDLE_ERROR(cudaMemcpy(T, c_Ma, n * sizeof(int), cudaMemcpyDeviceToHost));
		
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

	printf("\nNote: Cost = %d, means you can not reach the node from the current starting node\n", tWeight);

	free(Va);
	free(Ea);
	free(Wa);
	free(T);
	cudaFree(c_Va);
	cudaFree(c_Ea);
	cudaFree(c_Wa);
	cudaFree(c_Ma);
	cudaFree(c_Ua);
	cudaFree(c_Ca);

	return 0;

}
