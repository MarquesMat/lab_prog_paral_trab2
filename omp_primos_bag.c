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

int main(int argc, char *argv[])
{
    double t_inicial, t_final;
    int total = 0;
    int n;
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

       while (tam * num_threads >= n)
    {
        tam -= 50000;
        
    }
    if (tam == 0 ) tam = 1;

    /* Registra o tempo inicial de execução do programa */
    t_inicial = omp_get_wtime();

    /* Inicia a região paralela */
    #pragma omp parallel num_threads(num_threads) shared(total, n)
    {
        int tid = omp_get_thread_num();
        int inicio, fim;

        /* Thread mestre (main) carrega a bag de tarefas */
        #pragma omp single
        {
            inicio = 3;
            while (inicio <= n)
            {
                #pragma omp task firstprivate(inicio) firstprivate(fim) if (tid != 0)
                {
                    int local_count = 0;
                    fim = inicio + tam-1;
                    if (fim > n) fim = n;
                    if (fim<inicio) fim = inicio;

                    for (int i = inicio; i <= fim; i += 2)
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

    } // Fim da região paralela

    /* Registra o tempo final de execução */
    t_final = omp_get_wtime();

    /* Imprime o resultado */
    total += 1; // Adiciona o número 2, que é primo
    printf("Quantidade de primos entre 1 e %d: %d\n", n, total);
    printf("Tempo de execução: %1.3f segundos\n", t_final - t_inicial);

    return 0;
}
