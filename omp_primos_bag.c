#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

// Retorna 1 se n for primo e 0 se não for
int primo(long int n) {
    if (n <= 1) return 0; // 0 e 1 não são primos
    if (n <= 3) return 1; // 2 e 3 são primos
    if (n % 2 == 0 || n % 3 == 0) return 0; // Múltiplos de 2 ou 3 não são primos

    for (long int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return 0;
    }
    return 1;
}

// Retorna o tempo marcado pela thread que terminou por último
double maior_tempo_final(int num_threads, double tempos[]) {
    double maior = tempos[0];
    for (int i = 1; i < num_threads; i++) {
        if (tempos[i] > maior) maior = tempos[i];
    }
    return maior;
}

int main(int argc, char *argv[]) {
    long int n, total = 0;
    int num_threads;
    double t_inicio;

    if (argc < 3) {
        printf("Argumento faltando! Entre com valores inteiros para N e número de threads, respectivamente.\n");
        return 0;
    } else {
        n = strtol(argv[1], (char **) NULL, 10);
        num_threads = strtol(argv[2], (char **) NULL, 10);
    }

    // Define o número de threads
    omp_set_num_threads(num_threads);

    double t_fim[num_threads];
    t_inicio = omp_get_wtime();

    #pragma omp parallel reduction(+:total)
    {
        int tid = omp_get_thread_num();

        if (tid == 0) {
            // Thread 0 aguarda
            #pragma omp barrier
        } else {
            // Outras threads pegam tarefas do "saco"
            #pragma omp for schedule(dynamic)
            for (long int i = 3; i <= n; i += 2) { // Só verifica números ímpares
                if (primo(i) == 1) total++;
            }
            t_fim[tid] = omp_get_wtime();
        }
    }

    // Acrescenta o número 2, que não foi verificado pelo loop
    if (n >= 2) total++;

    // Aguarda todas as threads terminarem antes de calcular o tempo total
    #pragma omp barrier

    double t_total = maior_tempo_final(num_threads, t_fim) - t_inicio;

    printf("Quantidade de primos entre 1 e %ld: %ld \n", n, total);    
    printf("Tempo total de execução: %3.10f segundos\n\n", t_total);

    return 0;
}
