#!/bin/bash

echo "Compilando o codigo..."
gcc -fopenmp -o openmp openmp.c
if [ $? -ne 0 ]; then
    echo "Erro na compilação!"
    read -p "Pressione Enter para continuar..."
    exit 1
fi
echo "Compilacao bem-sucedida!"
echo

echo "Testes para N fixo (10000), K variavel e Threads variavel"
for t in 1 2 4 8 16 32; do
    for k in 5 10 20 50 100; do
        echo "Testando N=10000, K=$k, Threads=$t"
        for i in {1..5}; do
            ./openmp 10000 $k $t
        done
        echo
    done
done

echo "Testes para K fixo (10), N variavel e Threads variavel"
for t in 1 2 4 8 16 32; do
    for n in 100 500 1000 10000; do
        echo "Testando N=$n, K=10, Threads=$t"
        for i in {1..5}; do
            ./openmp $n 10 $t
        done
        echo
    done
done