#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// #include <mpi.h>

int primo(long int n)
{ /* mpi_primos.c  */
	int i;

	for (i = 3; i < (int)(sqrt(n) + 1); i += 2)
	{
		if (n % i == 0)
			return 0;
	}
	return 1;
}

int main(int argc, char *argv[])
{
	double t_inicial, t_final;
	int cont = 0, total = 0;
	long int i, n;
	int meu_ranque, inicio, salto;
	clock_t start, end;

	if (argc < 2)
	{
		printf("Valor inválido! Entre com um valor do maior inteiro\n");
		return 0;
	}
	else
	{
		n = strtol(argv[1], (char **)NULL, 10);
	}

	// MPI_Init(&argc, &argv);

	//(MPI_COMM_WORLD, &meu_ranque);
	//  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
	// t_inicial = MPI_Wtime();
	start = clock();
	inicio = 3;

	for (i = inicio; i <= n; i += 2)
	{
		if (primo(i) == 1)
			cont++;
	}

	total = cont;
	/*if(num_procs > 1) {
		MPI_Reduce(&cont, &total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	} else {
		total = cont;
	}*/

	// t_final = MPI_Wtime();
	end = clock();
	double time_taken = (end - start) / (double)CLOCKS_PER_SEC;

	total += 1; /* Acrescenta o dois, que também é primo */
	printf("Quant. de primos entre 1 e n: %d \n", total);
	// printf("Tempo de execucao: %1.3f \n", t_final - t_inicial);
	printf("Tempo de execucao %1.3f", time_taken);

	// MPI_Finalize();
	return (0);
}