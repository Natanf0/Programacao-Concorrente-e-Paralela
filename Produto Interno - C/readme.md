Autor: Natan da S. Ferreira | Programação Concorente 2025.1 | UFRJ

Descrição:  ̃Sejam (a1, a2, . . . , aN )T e (b1, b2, . . . , bN )T vetores em um espaco de dimensão N expressos em termos de um sistema ortogonal de coordenadas cartesianas. 
O produto interno desses dois vetores e um valor real dado pela equação: a1*b1 + a2*b2 + . . . + aN*bN

Observação: Para os itens 2 e 5, acesse o link: https://docs.google.com/document/d/1LmogXJpzdneCenKPnTl6YlXpDl_UavgPQSf5YUkUW6Y/edit?usp=sharing 

Nesta atividade vamos calcular o produto interno de 2 formas:
1. Sequencial.
2. Concorrente.

Os arquivos:
1. "gerar_vetor.c" é o programa que gera um arquivo binário da seguinte forma:
       1 inteiro N representando o tamanho dos vetores A e B.
       N's floats randômicos que representam os vetores A e B, respectivamente.
       1 double que representa o cálculo do produto interno de A e B realizado de forma sequencial, ou seja, apenas com a thread main

   Após compilar, o programa deve ser executado na forma: ./<nome_do_executavel> <N> <nome_arquivo_saida.bin>

2. "produto_interno.c" é o programa que calcula o produto interno de forma concorrente. O programa consome o arquivo binário gerado anteriormente e o número de threads desejadas e calcula o produto interno de A e B, comparando os resultados com o cálculo sequencial realizado anteriormente.
  Após compilar, o programa deve ser executado na forma: ,/<nome_do_executavel> <nome_arquivo_saida.bin> <qtde_threads>

Fluxo: gcc -o gerar_vetor gerar_vetor.c
       gcc -o produto_interno produto_interno.c
       ./gerar_vetor 200 vetor.bin
       ./produto_interno vetor.bin 20
       