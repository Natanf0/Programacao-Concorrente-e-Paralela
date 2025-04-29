#!/bin/bash
gcc -o gerarMatriz geraMatrizBinario.c
gcc -o matvet matvet.c 

./gerarMatriz 45000 45000 matbig
./gerarMatriz 45000 1 vetbig

# Definindo os números de threads e as repetições
threads=(1 2 3 5 10 20)
repeticoes=(1 2 3 4 5 6 7 8 9 10)

# Arquivos de entrada
arquivomatriz="matbig"  # Nome do arquivo da matriz 45.000 x 45.000
arquivovetor="vetbig"   # Nome do arquivo do vetor 45.000 x 1

# Loop sobre os números de threads
echo "Carregamento, Execução, Desalocação, Total"
for i in ${threads[@]}; do
    echo "Executando com $i threads"
   for j in ${repeticoes[@]}; do
      # Executar o programa matvet com os parâmetros necessários
      ./matvet $arquivomatriz $arquivovetor $i
   done
   # Separar as execuções de threads diferentes
   echo ""
done
python3 plot.py
