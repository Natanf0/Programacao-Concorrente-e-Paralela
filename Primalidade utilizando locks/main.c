#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <math.h>
#include "timer.h"


long long int N;
long long int qtd_primos = 0, contador = 0 ;
int n_threads;

pthread_mutex_t mutex1;
pthread_mutex_t mutex2;

int ehPrimo(long long int n) {
    int i;
    if (n<=1) return 0;
    if (n==2) return 1;
    if (n%2==0) return 0;
    for (i=3; i<sqrt(n)+1; i+=2) {
        if(n%i==0) return 0; 
    }
    return 1;
}

void* contaPrimos(void* arg){
    long int id = (long int) arg;
    long long int copy_contador;

    long long int primos = 0; // variável local da thread

    do{
        pthread_mutex_lock(&mutex1);
        copy_contador = contador; 
        contador += 1;
        if(contador >= N) {
            pthread_mutex_unlock(&mutex1);
            break;
        }
        pthread_mutex_unlock(&mutex1);

        if(ehPrimo(copy_contador)) primos += 1;
    }while(1);
    //printf("copy = %lld\n", copy_contador);
    pthread_mutex_lock(&mutex2);
    qtd_primos += primos;
    pthread_mutex_unlock(&mutex2);
    
    pthread_exit(NULL);
}



int main(int argc, char *argv[]){
    double inicio, fim, delta;



    pthread_t *tid; //identificadores das threads no sistema
    int nthreads; //qtde de threads (passada linha de comando)

   //--le e avalia os parametros de entrada

    GET_TIME(inicio);
    if(argc<3) {
        printf("Digite: %s <N> <numero de threads>\n", argv[0]);
        return 1;
    }
    N = atoll(argv[1]);
    nthreads = atoi(argv[2]);

   //--aloca as estruturas
    tid = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
    if(tid==NULL) {
        puts("ERRO--malloc"); return 2;
    }
    pthread_mutex_init(&mutex1, NULL);
    pthread_mutex_init(&mutex2, NULL);

    for(long int t=0; t<nthreads; t++) {
        if (pthread_create(&tid[t], NULL, contaPrimos, (void *)t)) {
          printf("--ERRO: pthread_create()\n"); exit(-1);
        }
      }
   
      //--espera todas as threads terminarem
      for (int t=0; t<nthreads; t++) {
        if (pthread_join(tid[t], NULL)) {
            printf("--ERRO: pthread_join() \n"); exit(-1); 
        } 
      } 
    GET_TIME(fim);
    delta = fim - inicio;
    printf("%d, %lld, %lf, ", nthreads, N, delta); // tempo de execução
    pthread_mutex_destroy(&mutex1);
    pthread_mutex_destroy(&mutex2);
    printf("qtd primos = %lld\n", qtd_primos); 
    free(tid);
    return 0;
}