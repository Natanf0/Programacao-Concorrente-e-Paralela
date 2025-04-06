#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h> 
#include <math.h>  // Para usar fabs() para o valor absoluto de números flutuantes

long int n; // dimensão dos vetores

// vetores para o cálculo do produto interno:
float* vetorA;
float* vetorB; 

int t_threads; // quantidade de threads desejadas para a execução do progama


void* produtoInterno(void *tid){
    long int id = (long int) tid;
    double produtoInterno = 0., *resultado;

    int bloco = (int) n/t_threads;
    if(id==1){bloco+=(int) n%t_threads;} // Caso haja algum resto da divisão do tamanho dos vetores pelo número de threads
    // Então a primeira thread irá fazer essa diferença a mais (n mod t). A estratégia de escolher a primeira thread foi pelo uso otimizado do pipeline da CPU
    int inicio = id*bloco;
    int fim = inicio + bloco;

    // cálculo do produto interno entre os vetores A e B
    for(int i=inicio; i<fim ; i++ ){
        produtoInterno += vetorA[i]*vetorB[i];
    }

    // retornando o resultado:
    resultado = (double*) malloc(sizeof(double)); 
    if (resultado!=NULL) *resultado = produtoInterno;
    else printf("Erro de alocação do resultado na thread de ID = %ld \n", id);
    pthread_exit((void*) resultado);
    
}

// Função que calcula a variação relativa dos resultados dos cálculos dos produtos internos
double variacaoRelativa(double valor_concorrente, double valor_sequencial){
    if (valor_sequencial == 0) {printf("Produto interno nulo. Não é possível dividir por zero"); exit(-1);}
    
    return fabs((valor_concorrente - valor_sequencial) / valor_sequencial);
}


int main(int* argc, char* argv[]){
    FILE *arquivo; //arquivo de entrada
    size_t retA, retB; //retorno da funcao de leitura no arquivo de entrada
    double produtoSequencial; // produto interno calculadod e forma sequencial (gravado no arquivo)

    double produtointernoParalelo_total = 0.;
    double* produtoInternoParalelo_parcial; // parcial = por thread

    pthread_t *tid_sistema; //vetor de identificadores das threads no sistema

    //valida e recebe os valores de entrada
    if(argc < 3) { printf("Use: %s <arquivo de entrada> <numero de threads> \n", argv[0]); exit(-1); }

    //abre o arquivo de entrada populado com o valor de N, os double's randomicos dos vetores e o produto interno
    arquivo = fopen(argv[1], "rb");
    if(arquivo==NULL) { printf("Erro de alocação para o arquivo de entrada\n"); exit(-1); }

    //le o tamanho do vetor (primeira linha do arquivo)
    retA = fread(&n, sizeof(long int), 1, arquivo);
    if(!retA) {
        fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo \n");
        return 3;
    }

    //aloca espaco de memoria e carrega o vetor de entrada
    vetorA = (float*) malloc (sizeof(float) *n);
    vetorB = (float*) malloc (sizeof(float) *n);
    if(vetorA==NULL || vetorB==NULL) { printf("Erro de alocação para os vetores\n"); exit(-1); }
    
    retA = fread(vetorA, sizeof(float), n, arquivo);
    retB = fread(vetorB, sizeof(float), n, arquivo);
    if(retA < n || retB < n ){
        fprintf(stderr, "Erro de leitura dos elementos do vetor\n");
        return 4;
    }


    //le o numero de threads da entrada do usuario 
    t_threads = atoi(argv[2]);
    //limita o numero de threads ao tamanho do vetor
    if(t_threads>n) t_threads = n;

    //aloca espaco para o vetor de identificadores das threads no sistema
    tid_sistema = (pthread_t *) malloc(sizeof(pthread_t) * t_threads);
    if(tid_sistema==NULL) { printf("Erro de alocação nos identificadores das threads\n"); exit(-1); }

    //cria as threads
    for(long int i=0; i<t_threads; i++) {
        if (pthread_create(&tid_sistema[i], NULL, produtoInterno, (void*) i)) {
            printf("Erro ao criar as threads\n"); exit(-1);
        }
    }

    //espera todas as threads terminarem e calcula a soma total das threads
    //retorno = (float*) malloc(sizeof(float));
    for(int i=0; i<t_threads; i++) {
        if (pthread_join(tid_sistema[i], (void *) &produtoInternoParalelo_parcial)) {
            printf("Erro ao aguardar as threads\n"); exit(-1);
    }
        produtointernoParalelo_total += *produtoInternoParalelo_parcial;
        free(produtoInternoParalelo_parcial);
  }

    //imprime os resultados
    printf("Resultados do produto interno dos vetores A e B:\n");
    printf("Método concorrente                 = %.26lf\n", produtointernoParalelo_total);
    //le o somatorio registrado no arquivo
    retA = fread(&produtoSequencial, sizeof(double), 1, arquivo); 
    printf("Método sequencial                  = %.26lf\n", produtoSequencial);

    printf("Variação relativa entre os métodos = %.26lf%\n\n", variacaoRelativa(produtointernoParalelo_total, produtoSequencial));

    //desaloca os espacos de memoria
    free(vetorA);
    free(vetorB);
    free(tid_sistema);
    //fecha o arquivo
    fclose(arquivo);
  
  return 0;  
}