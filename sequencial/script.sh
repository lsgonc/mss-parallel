#!/bin/bash

echo "Testes para N fixo (5000) e K variavel"

for k in 5 10 20 50 100; do
    echo "Testando N=5000, K=$k"
    for i in {1..5}; do
        ./sequencial 10000 $k
    done
done

echo "Testes para K fixo (10) e N variavel"

for n in 100 500 1000 5000; do
    echo "Testando N=$n, K=10"
    for i in {1..5}; do
        ./sequencial $n 10
    done
done