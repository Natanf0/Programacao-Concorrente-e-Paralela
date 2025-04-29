#!/bin/bash
gcc -o gerarMatriz geraMatrizBinario.c
gcc -o matmat matmat.c 

echo "Gerando a matriz A"
./gerarMatriz 45000 45000 matA 0

echo ""
echo "Gerando e transpondo a matriz B"
./gerarMatriz 45000 45000 matB 1

echo ""
echo "Agora, execute o comando: bash processamento.sh"
