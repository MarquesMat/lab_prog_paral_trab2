#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define TAMANHO 500000

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

int main(int argc, char *argv[])
{
    double t_inicio;
    long int n, total = 0;
    int num_threads;
    int tam = TAMANHO;

    /* Verifica o número de argumentos passados */
    if (argc < 2)
    {
        printf("Entre com o valor do maior inteiro como parâmetro para o programa.\n");
        return 0;
    }
    else
    {
        n = strtol(argv[1], (char **)NULL, 10);
    }

    /* Define o número de threads */
    num_threads = omp_get_max_threads();
    double t_fim[num_threads]; // Declare array after num_threads is set

    while (tam > 50000 && tam * num_threads >= n )
    {
        tam -= 50000;
    }
    
    /* Registra o tempo inicial de execução do programa */
    t_inicio = omp_get_wtime();

    /* Inicia a região paralela */
    #pragma omp parallel num_threads(num_threads) shared(total, n, tam, t_fim)
    {
        int tid = omp_get_thread_num();
        double thread_start_time = omp_get_wtime();

        /* Thread mestre (main) carrega a bag de tarefas */
        #pragma omp single nowait 
        {
            int inicio = 3;
            while (inicio <= n)
            {
                #pragma omp task firstprivate(inicio) private(tid)
                {
                    int local_count = 0;
                    int fim = inicio + tam - 1;
                    if (fim < inicio) fim = inicio;
                    for (int i = inicio; i <= fim && i < n; i += 2)
                    {
                        if (primo(i))
                            local_count++;
                    }
                    #pragma omp atomic
                    total += local_count;
                }
                inicio += tam;
            }
        } // Fim da região single

        t_fim[tid] = omp_get_wtime() - thread_start_time;
    } // Fim da região paralela

    // Acrescenta o número 2, que não foi verificado pelo loop
    if (n >= 2) total++;

    double t_total = maior_tempo_final(num_threads, t_fim);

    printf("Quantidade de primos entre 1 e %ld: %ld \n", n, total);    
    printf("Tempo total de execução: %3.10f segundos\n\n", t_total);

    return 0;
}
