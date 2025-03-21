#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int MaxIncreasingSub(int arr[], int n, int k, int num_threads) {
    int **dp, ans = -1;
    dp = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        dp[i] = (int *)malloc((k + 1) * sizeof(int));
        for (int j = 0; j <= k; j++) {
            dp[i][j] = -1;
        }
    }

    for (int i = 0; i < n; i++) {
        dp[i][1] = arr[i];
    }

    clock_t start_time = clock();

    // Primeiras 256 iterações sequenciais (ou até n se n < 256)
    int sequential_limit = (n < 256) ? n : 256;
    for (int i = 1; i < sequential_limit; i++) {
        for (int j = 0; j < i; j++) {
            if (arr[j] < arr[i]) {
                for (int l = 1; l <= k - 1; l++) {
                    if (dp[j][l] != -1) {
                        if (dp[i][l + 1] < dp[j][l] + arr[i]) {
                            dp[i][l + 1] = dp[j][l] + arr[i];
                        }
                    }
                }
            }
        }
    }

    // Restante das iterações em paralelo
    if (n > 256) {
        #pragma omp parallel for
        for (int i = 256; i < n; i++) {
            for (int j = 0; j < i; j++) {
                if (arr[j] < arr[i]) {
                    for (int l = 1; l <= k - 1; l++) {
                        if (dp[j][l] != -1) {
                            #pragma omp critical
                            if (dp[i][l + 1] < dp[j][l] + arr[i]) {
                                dp[i][l + 1] = dp[j][l] + arr[i];
                            }
                        }
                    }
                }
            }
        }
    }

    #pragma omp parallel for reduction(max: ans)
    for (int i = 0; i < n; i++) {
        if (dp[i][k] > ans) {
            ans = dp[i][k];
        }
    }

    clock_t end_time = clock();
    double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC * 1000;

    for (int i = 0; i < n; i++) {
        free(dp[i]);
    }
    free(dp);

    printf("N = %d; K = %d; THREADS = %d; TEMPO: %f ms; RESULTADO: %d\n", 
           n, k, num_threads, time_taken, (ans == -1) ? 0 : ans);

    return (ans == -1) ? 0 : ans;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Uso: %s <n> <k> <threads>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    int k = atoi(argv[2]);
    int num_threads = atoi(argv[3]);

    omp_set_num_threads(num_threads);

    if (n <= 0 || k <= 0 || num_threads <= 0) {
        printf("Erro: n, k e threads devem ser positivos\n");
        return 1;
    }

    int arr[n];
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 1000;
    }

    int result = MaxIncreasingSub(arr, n, k, num_threads);

    return 0;
}