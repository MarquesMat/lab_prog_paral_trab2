#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

int primo (long int n) { 
	int i;   
	for (i = 3; i < (int)(sqrt(n) + 1); i+=2) {
			if(n%i == 0) return 0;
	}
	return 1;
}

int main(int argc, char *argv[]) {
	double t_inicial, t_final;
	int total = 0;
	int i, n, t;

	if (argc < 3) {
		printf("Valor inválido! Entre com um valor do maior inteiro e com o numero de threads\n");
		return 0;
	} else {
		n = strtol(argv[1], (char **) NULL, 10);
        t = strtol(argv[2], (char **) NULL, 10);
	}

    omp_set_num_threads(t);
	t_inicial = omp_get_wtime();


		#pragma omp parallel for reduction(+:total) schedule(dynamic, 500000)
		for (i = 3; i <= n; i+=2) {
			if(primo(i) == 1) total++;
		}

	total += 1; //Adds 2, which is also a prime number
	t_final = omp_get_wtime();

	printf("Quant. de primos entre 1 e %d: %d \n", n, total);
	printf("Tempo de execucao: %1.3f \n", t_final - t_inicial);	

	return(0);
}