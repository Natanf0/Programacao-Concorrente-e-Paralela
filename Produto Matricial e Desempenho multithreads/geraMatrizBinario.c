/* Programa que cria arquivo com uma matriz de valores do tipo float, gerados aleatoriamente 
 * Entrada: dimensoes da matriz (linhas e colunas) e nome do arquivo de saida
 * Saida: arquivo binario com as dimensoes (valores inteiros) da matriz (linhas e colunas), 
 * seguido dos valores (float) de todas as celulas da matriz gerados aleatoriamente
 * */

#include<stdio.h>
#include<stdlib.h>
#include<time.h>

//descomentar o define abaixo caso deseje imprimir uma versao truncada da matriz gerada no formato texto
//#define TEXTO 

int main(int argc, char*argv[]) {
   float *matriz; //matriz que será gerada
   float *matrizTransposta; // # Modifiquei o código para incluir a matriz transposta. Facilitará o cálculo matriz x matriz
   
   int linhas, colunas; //dimensoes da matriz
   long long int tam; //qtde de elementos na matriz

   FILE * descritorArquivo; //descritor do arquivo de saida
   FILE *descritorTransposta;
   size_t ret; //retorno da funcao de escrita no arquivo de saida
   
   //recebe os argumentos de entrada
   if(argc < 4) {
      fprintf(stderr, "Digite: %s <linhas> <colunas> <arquivo saida> <gerar transposta? (0 / 1)>\n", argv[0]);
      return 1;
   }
   linhas = atoi(argv[1]); 
   colunas = atoi(argv[2]);
   tam = linhas * colunas;

   //aloca memoria para a matriz
   matriz = (float*) malloc(sizeof(float) * tam);
   
   if(!matriz) {
      fprintf(stderr, "Erro de alocao da memoria da matriz\n");
      return 2;
   }

   //preenche a matriz com valores float aleatorios
   //randomiza a sequencia de numeros aleatorios
   srand(time(NULL));
   for(long int i=0; i<tam; i++) {
       *(matriz+i) = (rand() % 1000) * 0.3;
   }
  
      descritorArquivo = fopen(argv[3], "wb");
      if(!descritorArquivo) {
         fprintf(stderr, "Erro de abertura do arquivo\n");
         return 3;
      }
   //escreve numero de linhas e de colunas
      ret = fwrite(&linhas, sizeof(int), 1, descritorArquivo);
      ret = fwrite(&colunas, sizeof(int), 1, descritorArquivo);
   //escreve os elementos da matriz
      ret = fwrite(matriz, sizeof(float), tam, descritorArquivo);
      if(ret < tam) {
         fprintf(stderr, "Erro de escrita no  arquivo\n");
         return 4;
      }

      fclose(descritorArquivo);
   



   if(argv[4]){
      float* matrizTransposta;
      matrizTransposta = (float*) malloc(sizeof(float) * tam);

      if(!matrizTransposta) {
         fprintf(stderr, "Erro de alocao da memoria da matriz transposta\n");
         return 5;
      }

      for(int i = 0; i<colunas; i++ ){
         for(int j=0; j<linhas; j++){
            matrizTransposta[i*colunas+j] = matriz[j*colunas+i];
         }
      }

      descritorTransposta = fopen("matBT", "wb");
      if(!descritorTransposta) {
         fprintf(stderr, "Erro de abertura do arquivo da matriz transposta n");
         return 6;
      }
      //escreve numero de linhas e de colunas
      ret = fwrite(&linhas, sizeof(int), 1, descritorTransposta);
      ret = fwrite(&colunas, sizeof(int), 1, descritorTransposta);
      ret = fwrite(matrizTransposta, sizeof(float), tam, descritorTransposta);
      if(ret < tam) {
         fprintf(stderr, "Erro de escrita no  arquivo da matriz transposta\n");
         return 7;
         }
   free(matrizTransposta);
   }

   //finaliza o uso das variaveis
   free(matriz);
   return 0;
}

