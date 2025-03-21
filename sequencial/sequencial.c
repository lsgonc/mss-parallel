#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>


int MaxIncreasingSub(int arr[], int n, int k) {
    // Alocação dinâmica correta para dp
    int **dp = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        dp[i] = (int *)malloc((k + 1) * sizeof(int));
        for (int j = 0; j <= k; j++) {
            dp[i][j] = -1;
        }
        dp[i][1] = arr[i];
    }

    // Medição apenas dos loops principais
    clock_t start_time = clock();
    for (int i = 1; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (arr[j] < arr[i]) {
                for (int l = 1; l <= k - 1; l++) {
                    if (dp[j][l] != -1) {
                        dp[i][l + 1] = (dp[i][l + 1] > dp[j][l] + arr[i]) ? dp[i][l + 1] : (dp[j][l] + arr[i]);
                    }
                }
            }
        }
    }

    // Busca pelo resultado
    int ans = -1;
    for (int i = 0; i < n; i++) {
        if (ans < dp[i][k]) {
            ans = dp[i][k];
        }
    }

    clock_t end_time = clock();


    // Tempo apenas dos loops
    double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC * 1000; // Em milissegundos

    // Liberação da memória
    for (int i = 0; i < n; i++) {
        free(dp[i]);
    }
    free(dp);

    printf("N = %d; K = %d; TEMPO: %f ms; RESULTADO: %d\n", n, k, time_taken, (ans == -1) ? 0 : ans);
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
    
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 1000;
    }

    int result = MaxIncreasingSub(arr, n, k);
    return 0;
}