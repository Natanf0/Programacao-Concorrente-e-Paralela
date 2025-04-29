/* Multiplicacao de matriz-vetor (considerando matrizes quadradas) */
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include "timer.h"

float *matA; // matriz de entrada A
float *matB; // matriz de entrada B transposta
float *matrizSaida; //matriz de saida

typedef struct{
   int id; //identificador do elemento que a thread ira processar
   int dim; //dimensao das estruturas de entrada (matriz quadrada)
   int nthreads; //numero de threads
} tArgs;

//funcao das threads
void * tarefa(void *arg) {
   tArgs *args = (tArgs*) arg;
   for(int i = args->id; i < args->dim; i += args->nthreads){
      for(int j = 0; j < args->dim; j++) 
         //matrizSaida[i*(args->dim) + j] += matA[i*(args->dim) + j] * matBT[i*(args->dim) + j];
         matrizSaida[i*(args->dim) + j] += matA[i*(args->dim) + j] * matB[j*(args->dim) + i];
   }   
   pthread_exit(NULL);
}

//carrega a matriz/vetor de entrada, retorna a qtde de linhas e colunas e a matriz/vetor preenchido
int carregaEntrada(FILE * descritorArquivo, float ** entrada, int *linhas, int *colunas) {
   size_t ret; //retorno da funcao de leitura no arquivo de entrada
   int tam; //tamanho da matriz

   //le as dimensoes 
   ret = fread(linhas, sizeof(int), 1, descritorArquivo);
   if(!ret) {
      fprintf(stderr, "Erro de leitura das dimensoes no arquivo \n");
      return 1;
   }
   ret = fread(colunas, sizeof(int), 1, descritorArquivo);
   if(!ret) {
      fprintf(stderr, "Erro de leitura das dimensoes no arquivo \n");
      return 2;
   }
   tam = (*linhas) * (*colunas); //calcula a qtde de elementos

   //aloca memoria
   *entrada = (float*) malloc(sizeof(float) * tam);
   if(!entrada) {
      fprintf(stderr, "Erro de alocao da memoria\n");
      return 3;
   }

   //carrega a entrada
   ret = fread(*entrada, sizeof(float), tam, descritorArquivo);
   if(ret < tam) {
      fprintf(stderr, "Erro de leitura dos elementos da estrutura de entrada\n");
      return 4;
   }
   return 0;
}

//fluxo principal
int main(int argc, char* argv[]) {
   int nthreads; //numero de threads
   pthread_t *tid; //identificadores das threads no sistema
   tArgs *args; //identificadores locais das threads e dimensao
   double inicio, fim, delta, total=0;
   FILE * descritorArquivoMatriz, * descritorArquivoVetor; //descritores do arquivo de entrada
   int linhasMat, linhasVet;
   int colunasMat, colunasVet;


   GET_TIME(inicio);
   //leitura e avaliacao dos parametros de entrada
   if(argc<4) {
      printf("Digite: %s <arquivo matriz> <arquivo vetor> <numero de threads>\n", argv[0]);
      return 1;
   }

   //abre o arquivo com a matriz para leitura binaria
   descritorArquivoMatriz = fopen(argv[1], "rb");
   if(!descritorArquivoMatriz) {
      fprintf(stderr, "Erro de abertura do arquivo com a matriz\n");
      return 2;
   }
   //carrega a matriz
   if(carregaEntrada(descritorArquivoMatriz, &matA, &linhasMat, &colunasMat)) //'mat' esta no escopo global
      return 3;

   //abre o arquivo com o vetor para leitura binaria
   descritorArquivoVetor = fopen(argv[2], "rb");
   if(!descritorArquivoVetor) {
      fprintf(stderr, "Erro de abertura do arquivo com o vetor\n");
      return 2;
   }
   //carrega o vetor
   if(carregaEntrada(descritorArquivoVetor, &matB, &linhasVet, &colunasVet)) //'vet' esta no escopo global
      return 3;

   //verifica se as dimensoes do vetor e matriz sao compativeis (assume matriz quadrada)	   
   if(colunasMat != linhasVet) {
      fprintf(stderr, "Erro dimensão das entradas\n");
   }
   //limita o numero de threads ao numero de linhas da matriz
   nthreads = atoi(argv[3]);
   if (nthreads > linhasMat) nthreads=linhasMat;

   //aloca espaço para o vetor de saida
   matrizSaida = (float *) malloc(sizeof(float) * linhasMat*colunasVet);
   if (matrizSaida == NULL) {printf("ERRO--malloc\n"); return 2;}

   GET_TIME(fim);
   delta = fim - inicio;
   //printf("Dimensao, Nthreads, Inicialização, Multiplicação, Finalização, Total\n");
   printf("%lf, ", delta);
   total = delta;

   //-----------------------------------------
   //multiplicacao da matriz pelo vetor
   GET_TIME(inicio);
   //alocacao das estruturas
   tid = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
   if(tid==NULL) {puts("ERRO--malloc"); return 4;}
   args = (tArgs*) malloc(sizeof(tArgs)*nthreads);
   if(args==NULL) {puts("ERRO--malloc"); return 4;}
   //criacao das threads
   for(int i=0; i<nthreads; i++) {
      (args+i)->id = i;
      (args+i)->dim = colunasMat;
      (args+i)->nthreads = nthreads;
      if(pthread_create(tid+i, NULL, tarefa, (void*) (args+i))){
         puts("ERRO--pthread_create"); return 3;
      }
   } 
   //espera pelo termino da threads
   for(int i=0; i<nthreads; i++) {
      pthread_join(*(tid+i), NULL);
   }
   GET_TIME(fim)   
   delta = fim - inicio;
   printf("%lf, ", delta);
   total += delta;

   //-----------------------------------------
   //liberacao da memoria
   GET_TIME(inicio);
   free(matA);
   free(matB);
   free(matrizSaida);
   free(args);
   free(tid);
   GET_TIME(fim)   
   delta = fim - inicio;
   printf("%lf, ", delta);
   total += delta;
   printf("%lf\n", total);

   return 0;
}
