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

void definir_inicios(long int *ini, int num_threads, long int n, int tamanho) {
    int dest, inicio;
    for (dest=1, inicio=3; dest < num_threads && inicio < n; dest++, inicio += tamanho){
        ini[dest] = inicio;
    }
}

int definir_tamanho(int t, int num_threads, long int n) {
    if (t * (num_threads - 1) >= n) return definir_tamanho(t-(TAMANHO/10), num_threads, n);
    return t;
}

int main(int argc, char *argv[]) {
    long int n, total = 0;
    int num_threads, tid, inicio, salto, tamanho = TAMANHO;
    double t_inicio;

    if (argc < 3) {
        printf("Argumento faltando! Entre com valores inteiros para N e número de threads, respectivamente.\n");
        return 0;
    } else {
        n = strtol(argv[1], (char **) NULL, 10);
        num_threads = strtol(argv[2], (char **) NULL, 10);
    }


    tamanho = definir_tamanho(TAMANHO, num_threads, n);
    printf("\nNovo tamanho: %d\n",tamanho);
    double t_fim[num_threads];
    t_inicio = omp_get_wtime();

    // Thread master define os inícios dos demais processos
    long int ini[num_threads];
    definir_inicios(ini, num_threads, n, tamanho);
    for (int i=1; i<num_threads; i++) printf("--> %ld\n", ini[i]);
    /*
    #pragma omp parallel num_threads(num_threads) private(tid, inicio, salto) reduction(+:total)
    {
        tid = omp_get_thread_num();
        inicio = 3 + (2 * tid); // Início do loop para cada thread
        salto = 2 * num_threads; // Salto para cada thread

        // Loop para verificar primos
        for (long int i = inicio; i <= n; i += salto) {
            if (primo(i) == 1) total++;
        }

        t_fim[tid] = omp_get_wtime();
    }

    // Acrescenta o número 2, que não foi verificado pelo loop
    if (n >= 2) total++;

    double t_total = maior_tempo_final(num_threads, t_fim) - t_inicio;

    printf("Quantidade de primos entre 1 e %ld: %ld \n", n, total);    
    printf("Tempo total de execução: %3.10f segundos\n\n", t_total);
    */
    return 0;
}
