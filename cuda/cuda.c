#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>

__global__ void computeDP(int *arr, int *dp, int n, int k, int i) {
    int j = blockIdx.x * blockDim.x + threadIdx.x;
    if (j >= i) return;

    if (arr[j] < arr[i]) {
        for (int l = 1; l <= k - 1; l++) {
            if (dp[j * (k + 1) + l] != -1) {
                int new_val = dp[j * (k + 1) + l] + arr[i];
                atomicMax(&dp[i * (k + 1) + (l + 1)], new_val);
            }
        }
    }
}

int MaxIncreasingSub(int *arr, int n, int k) {
    int *d_arr, *d_dp;
    size_t sizeArr = n * sizeof(int);
    size_t sizeDP = n * (k + 1) * sizeof(int);

    cudaMalloc((void **)&d_arr, sizeArr);
    cudaMalloc((void **)&d_dp, sizeDP);

    int *h_dp = (int *)malloc(sizeDP);
    for (int i = 0; i < n * (k + 1); i++) {
        h_dp[i] = -1;
    }
    for (int i = 0; i < n; i++) {
        h_dp[i * (k + 1) + 1] = arr[i];
    }

    cudaMemcpy(d_dp, h_dp, sizeDP, cudaMemcpyHostToDevice);
    cudaMemcpy(d_arr, arr, sizeArr, cudaMemcpyHostToDevice);


    for (int i = 1; i < n; i++) {
        int threadsPerBlock = 256;
        int blocksPerGrid = (i + threadsPerBlock - 1) / threadsPerBlock;
        computeDP<<<blocksPerGrid, threadsPerBlock>>>(d_arr, d_dp, n, k, i);
        cudaDeviceSynchronize();
    }

    cudaMemcpy(h_dp, d_dp, sizeDP, cudaMemcpyDeviceToHost);

    int ans = -1;
    for (int i = 0; i < n; i++) {
        if (h_dp[i * (k + 1) + k] > ans) {
            ans = h_dp[i * (k + 1) + k];
        }
    }

    cudaFree(d_arr);
    cudaFree(d_dp);
    free(h_dp);

    return (ans == -1) ? 0 : ans;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s <n> <k>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    int k = atoi(argv[2]);

    if (n <= 0 || k <= 0) {
        printf("Erro: n e k devem ser positivos\n");
        return 1;
    }

    // Usa a matriz pré-definida, limitada por n
    int arr[n];

    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 1000;
    }

    cudaEvent_t start, stop;
    float time;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    cudaEventRecord(start, 0);

    int result = MaxIncreasingSub(arr, n, k);

    cudaEventRecord(stop, 0);
    cudaEventSynchronize(stop);
    cudaEventElapsedTime(&time, start, stop);
    cudaEventDestroy(start);
    cudaEventDestroy(stop);

    printf("n = %d; k = %d; TEMPO_EXECUCAO: %f ms; RESULTADO: %d\n", n, k, time, result);

    return 0;
}