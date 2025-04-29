#!/bin/bash
# Definindo os números de threads e as repetições
threads=(1 2 3 5 10 20)
repeticoes=(1 2 3 4 5 6 7 8 9 10)

# Arquivos de entrada
arquivoMatA="matA"  # Nome do arquivo da matriz A 45.000 x 45.000
arquivoMatBT="matBT"   # Nome do arquivo da matriz B 45.000 x 45000 transposta

# Loop sobre os números de threads
echo "Carregamento, Execução, Desalocação, Total"
for i in ${threads[@]}; do
    echo "Executando com $i threads"
   for j in ${repeticoes[@]}; do
      # Executar o programa matvet com os parâmetros necessários
      ./matmat $arquivoMatA $arquivoMatBT $i
   done
   # Separar as execuções de threads diferentes
   echo ""
done
