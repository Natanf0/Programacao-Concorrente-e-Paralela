// Natan da S. Ferreira | 120027447
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <semaphore.h>

long long int n;
int m, c;

long* canal; // canal onde serão depositados os inteiros
int in = 0, out = 0;

sem_t leitura;
sem_t escrita;
sem_t mutex;
sem_t mutex2;

int ja_consumidos = 0;
int* qtd_primos_por_thread;
int qtd_primos = 0;

int ehPrimo(long long int n) {
    if (n <= 1) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    for (int i = 3; i <= sqrt(n); i += 2)
        if (n % i == 0) return 0;
    return 1;
}

void* consumidora(void* arg) {
    long id = (long) arg;
    while (1) {
        sem_wait(&leitura);
        sem_wait(&mutex);

        // Verifica se já consumimos todos
        if (ja_consumidos >= n) {
            sem_post(&mutex);  // libera mutex antes de sair
            sem_post(&leitura); // devolve a leitura para outras threads que podem sair
            break;
        }

        int num = canal[out];
        out = (out + 1) % m;
        ja_consumidos++;

        sem_post(&escrita);
        sem_post(&mutex);

        sem_wait(&mutex2);
        if (ehPrimo(num)) {
            qtd_primos_por_thread[id]++;
            qtd_primos++; 
        }
        sem_post(&mutex2);
    }
    return NULL;
}

void* produtora(void *arg) {
    for (long i = 0; i < n; i++) {
        sem_wait(&escrita);

        canal[in] = i;
        in = (in + 1) % m;

        sem_post(&leitura);
    }

    // Após terminar, libera o semáforo leitura para permitir que threads saiam
    for (int i = 0; i < c; i++) {
        sem_post(&leitura);
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Digite: %s <N> <M> <C>\n", argv[0]);
        return 1;
    }

    n = atoll(argv[1]);
    m = atoi(argv[2]); // tamanho do canal
    c = atoi(argv[3]); // número de threads consumidoras

    canal = (long*) malloc(sizeof(long) * m);
    qtd_primos_por_thread = (int*) calloc(c, sizeof(int));

    if (!canal || !qtd_primos_por_thread) {
        puts("ERRO--malloc");
        return 2;
    }

    sem_init(&leitura, 0, 0);
    sem_init(&escrita, 0, m);
    sem_init(&mutex, 0, 1);
    sem_init(&mutex2, 0, 1);

    pthread_t *tid = (pthread_t*) malloc(sizeof(pthread_t) * (c + 1));
    if (!tid) {
        puts("ERRO--malloc");
        return 3;
    }

    // cria a thread produtora
    if (pthread_create(&tid[0], NULL, produtora, NULL)) {
        printf("--ERRO: pthread_create() produtora\n"); exit(-1);
    }

    // cria as threads consumidoras
    for (long t = 1; t <= c; t++) {
        if (pthread_create(&tid[t], NULL, consumidora, (void *)(t - 1))) {
            printf("--ERRO: pthread_create() consumidora\n"); exit(-1);
        }
    }

    pthread_join(tid[0], NULL);
    for (int t = 1; t <= c; t++) {
        pthread_join(tid[t], NULL);
    }

    // Determina a thread vencedora
    int vencedora = 0;
    for (int i = 1; i < c; i++) {
        if (qtd_primos_por_thread[i] > qtd_primos_por_thread[vencedora]) {
            vencedora = i;
        }
    }

    printf("\nTotal de primos encontrados: %d\n", qtd_primos);
    printf("Thread consumidora vencedora: #%d (com %d primos)\n",
           vencedora, qtd_primos_por_thread[vencedora]);

    free(canal);
    free(qtd_primos_por_thread);
    free(tid);
    sem_destroy(&leitura);
    sem_destroy(&escrita);
    sem_destroy(&mutex);
    return 0;
}
