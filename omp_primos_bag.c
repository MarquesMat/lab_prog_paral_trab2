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
int primo (long int n) {
    for (long int i = 3; i < (long int)(sqrt(n) + 1); i+=2) if (n%i == 0) return 0;
    return 1;
}

// Ajusta o tamanho de trabalho para evitar deadlock
int definir_tamanho(int t, int num_threads, long int n) {
    if (t * (num_threads - 1) >= n) return definir_tamanho(t-(TAMANHO/10), num_threads, n); // Reduz em 10% do valor original a cada iteração
    return t;
}

int main(int argc, char *argv[])
{
    double t_inicio, t_final;
    long int n, total = 0;
    int num_threads;
    int tam = TAMANHO;

    if (argc < 3) {
        printf("Argumento faltando! Entre com valores inteiros para N e número de threads, respectivamente.\n");
        return 0;
    } else {
        n = strtol(argv[1], (char **) NULL, 10);
        num_threads = strtol(argv[2], (char **) NULL, 10);
    }

    tam = definir_tamanho(TAMANHO, num_threads, n);
    printf("\nNovo tamanho: %d\n",tam);

    t_inicio = omp_get_wtime();
    
    #pragma omp parallel for num_threads(num_threads) reduction(+:total) schedule(dynamic, tam)
		for (int i = 3; i <= n; i+=2) {
			if(primo(i) == 1) total++;
		}

    t_final = omp_get_wtime();


    // Acrescenta o número 2, que não foi verificado pelo loop
    total++;

    printf("Quantidade de primos entre 1 e %ld: %ld \n", n, total);    
    printf("Tempo total de execução: %3.10f segundos\n\n", t_final - t_inicio);

    return 0;
}
