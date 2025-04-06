/*
Programa auxiliar para gerar dois vetores de floats 
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 100 //valor maximo de um elemento do vetor
//descomentar o define abaixo caso deseje imprimir uma versao do vetor gerado no formato texto
//#define TEXTO 

int main(int argc, char*argv[]) {
   float *vetorA, *vetorB; //vetores que serão gerados
   long int n; //qtde de elementos do vetor
   float valorGeradoA, valorGeradoB; //valor gerado para incluir no vetor
   double produtoInterno = 0; //soma total dos elementos gerados
   int fator=1; //fator multiplicador para gerar números negativos
   FILE * descritorArquivo; //descritor do arquivo de saida
   size_t retA, retB, retC; //retorno da funcao de escrita no arquivo de saida

   //recebe os argumentos de entrada
   if(argc < 3) {
      fprintf(stderr, "Digite: %s <dimensao> <nome arquivo saida>\n", argv[0]);
      return 1;
   }
   n = atoi(argv[1]);

   //aloca memoria para o vetor
   vetorA = (float*) malloc(sizeof(float) * n);
   vetorB = (float*) malloc(sizeof(float) * n);
   if((!vetorA) || (!vetorB)) {
      fprintf(stderr, "Erro de alocao da memoria do vetor\n");
      return 2;
   }

   //preenche os vetores com valores float aleatórios
   srand(time(NULL));
   for(long int i=0; i<n; i++) {
        valorGeradoA = (rand() % MAX)/2.0 * fator;
        vetorA[i] = valorGeradoA;
        fator*=-1;
        valorGeradoB = (rand() % MAX)/2.0 * fator;
        vetorB[i] = valorGeradoB;
        
   }

   // Cálculo do produto interno sequencial:
   for(long int i=0; i<n; i++) {
      produtoInterno+=vetorA[i]*vetorB[i];
   }
   

   //imprimir na saida padrao o vetor gerado
   #ifdef TEXTO
   fprintf(stdout, "%ld\n", n);
   for(long int i=0; i<n; i++) {
      fprintf(stdout, "%f ",vetor[i]);
   }
   fprintf(stdout, "\n");
   fprintf(stdout, "%lf\n", soma);
   #endif

   //escreve o vetor no arquivo
   //abre o arquivo para escrita binaria
   descritorArquivo = fopen(argv[2], "wb");
   if(!descritorArquivo) {
      fprintf(stderr, "Erro de abertura do arquivo\n");
      return 3;
   }
   //escreve a dimensao
   retA = fwrite(&n, sizeof(long int), 1, descritorArquivo);
   //escreve os elementos do vetor
   retB = fwrite(vetorA, sizeof(float), n, descritorArquivo);
   retC = fwrite(vetorB, sizeof(float), n, descritorArquivo);
   if(retA < 1 || retB < n || retC < n) {
      fprintf(stderr, "Erro de escrita no  arquivo\n");
      return 4;
   }
   //escreve o somatorio
   retA = fwrite(&produtoInterno, sizeof(double), 1, descritorArquivo);

   //finaliza o uso das variaveis
   fclose(descritorArquivo);
   free(vetorA);
   free(vetorB);
   return 0;
} 
