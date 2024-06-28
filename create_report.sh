#!/bin/bash

report="report_${1}.txt"
script="${1}.c"
echo "INÍCIO DO PROGRAMA: $(date "+ %H:%M:%S")" > $report
echo -e "\nResultados para $script\n" >> $report
for n in {1000000,10000000,100000000,1000000000}; do
    echo "Rodando os programas com N = $n" >> $report
    for i in {1,2,3}; do
        echo "--- RODADA $i ---" >> $report
        for p in {8,4,2}; do
            echo -e "\nRodando os programas com $p processos\n" >> $report
            gcc $script -fopenmp -o $1 -lm
            ./primos_naive $n $p >> $report
            rm $1
            sleep 30
        done
    done
done
echo -e "\nFIM DO PROGRAMA: $(date "+ %H:%M:%S")" >> $report