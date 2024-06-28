#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define TAMANHO 500000

/*EXEMPLOS DE EXECUÇÃO
gcc omp_primos_bag.c -fopenmp -o primos_bag -lm
Com 10^6 e 2 threads: ./primos_bag 1000000 2
Com 10^6 e 4 threads: ./primos_bag 1000000 4
Com 10^8 e 4 threads: ./primos_bag 100000000 4
Com 10^8 e 8 threads: ./primos_bag 100000000 8
*/

// Retorna 1 se n for primo e 0 se não for
int primo(long int n)
{
    for (long int i = 3; i < (long int)(sqrt(n) + 1); i += 2)
        if (n % i == 0)
            return 0;
    return 1;
}

// Retorna o tempo marcado pela thread que terminou por último
double maior_tempo_final(int num_threads, double tempos[])
{
    double maior = tempos[0];
    for (int i = 1; i < num_threads; i++)
    {
        if (tempos[i] > maior)
            maior = tempos[i];
    }
    return maior;
}

// Ajusta o tamanho de trabalho para evitar deadlock
int definir_tamanho(int t, int num_threads, long int n)
{
    if (t * (num_threads - 1) >= n)
        return definir_tamanho(t - (TAMANHO / 10), num_threads, n); // Reduz em 10% do valor original a cada iteração
    return t;
}

int main(int argc, char *argv[])
{
    double t_inicio;
    long int n, total = 0;
    int num_threads;
    int tam = TAMANHO;

    if (argc < 3)
    {
        printf("Argumento faltando! Entre com valores inteiros para N e número de threads, respectivamente.\n");
        return 0;
    }
    else
    {
        n = strtol(argv[1], (char **)NULL, 10);
        num_threads = strtol(argv[2], (char **)NULL, 10);
    }

    tam = definir_tamanho(TAMANHO, num_threads, n);
    printf("\nNovo tamanho: %d\n", tam);

    double t_fim[num_threads];
    t_inicio = omp_get_wtime();

/* Inicia a região paralela */
#pragma omp parallel num_threads(num_threads) shared(total, n, tam, t_fim)
    {
        int tid = omp_get_thread_num(); // Essa variável não está sendo usada, pois a função get foi chamada abaixo

/* Thread mestre (main) carrega a bag de tarefas */
#pragma omp single nowait // Não sei como funciona isso
        {
            printf("ID da master: %d\n", omp_get_thread_num());
            int inicio = 3;
            while (inicio <= n)
            {
#pragma omp task firstprivate(inicio) private(tid) // Não sei como funciona isso
                {
                    printf("Thread %d: inicio %d\n", omp_get_thread_num(), inicio); // Mostra os valores de início dados a cada thread
                    int local_count = 0;
                    int fim = inicio + tam - 1;
                    if (fim < inicio)
                        fim = inicio;
                    for (int i = inicio; i <= fim && i < n; i += 2)
                    {
                        if (primo(i))
                            local_count++; // O ideal seria usar a cláusula reduction e acessar diretamento o total
                    }
#pragma omp atomic
                    total += local_count;
                }
                inicio += tam;
            }
        } // Fim da região single

        t_fim[tid] = omp_get_wtime(); // parece q tá pegando só o tempo da master, mas não conferi
    } // Fim da região paralela

    // Acrescenta o número 2, que não foi verificado pelo loop
    total++;

    // double t_total = maior_tempo_final(num_threads, t_fim);

    printf("Quantidade de primos entre 1 e %ld: %ld \n", n, total);
    printf("Tempo total de execução: %3.10f segundos\n\n", maior_tempo_final(num_threads, t_fim) - t_inicio);

    return 0;
}
