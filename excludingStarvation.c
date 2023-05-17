#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int readerCount = 0; // Liczba czytelników w czytelni
int writerCount = 0; // Liczba pisarzy w czytelni
int readerQueue = 0; // Liczba czytelników w kolejce przed czytelnią
int writerQueue = 0; // Liczba pisarzy w kolejce przed czytelnią

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t readerCondition = PTHREAD_COND_INITIALIZER;
pthread_cond_t writerCondition = PTHREAD_COND_INITIALIZER;

void* reader(void* arg) {
    int readerID = *((int*)arg);
    free(arg);

    while (1) {
        pthread_mutex_lock(&mutex);
        readerQueue++;

        while (writerCount > 0 || writerQueue > 0) {
            pthread_cond_wait(&readerCondition, &mutex);
        }

        readerQueue--;
        readerCount++;
        printf("ReaderQ: %d WriterQ: %d [in: R:%d W:%d]\n", readerQueue, 
writerQueue, readerCount, writerCount);

        pthread_cond_signal(&readerCondition);

        pthread_mutex_unlock(&mutex);

        // Czytanie

        pthread_mutex_lock(&mutex);
        readerCount--;

        if (readerCount == 0 && writerQueue > 0) {
            pthread_cond_signal(&writerCondition);
        }

        pthread_mutex_unlock(&mutex);

        // Powrót do czytelni
    }
}

void* writer(void* arg) {
    int writerID = *((int*)arg);
    free(arg);

    while (1) {
        pthread_mutex_lock(&mutex);
        writerQueue++;

        while (readerCount > 0 || writerCount > 0) {
            pthread_cond_wait(&writerCondition, &mutex);
        }

        writerQueue--;
        writerCount++;
        printf("ReaderQ: %d WriterQ: %d [in: R:%d W:%d]\n", readerQueue, 
writerQueue, readerCount, writerCount);

        pthread_cond_signal(&writerCondition);

        pthread_mutex_unlock(&mutex);

        // Pisanie

        pthread_mutex_lock(&mutex);
        writerCount--;

        if (readerQueue > 0) {
            pthread_cond_signal(&readerCondition);
        } else if (writerQueue > 0) {
            pthread_cond_signal(&writerCondition);
        }

        pthread_mutex_unlock(&mutex);

        // Powrót do czytelni
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Podaj liczbę czytelników (W) i pisarzy (R) jako argumenty 
programu.\n");
        return 1;
    }

    int readerThreads = atoi(argv[1]);
    int writerThreads = atoi(argv[2]);

    pthread_t* readers = malloc(readerThreads * sizeof(pthread_t));
    pthread_t* writers = malloc(writerThreads * sizeof(pthread_t));

    for (int i = 0; i < readerThreads; i++) {
        int* readerID = malloc(sizeof(int));
        *readerID = i + 1;
        pthread_create(&readers[i], NULL, reader, readerID);
    }

    for (int i = 0; i < writerThreads; i++) {
        int* writerID = malloc(sizeof(int));
        *writerID = i + 1;
        pthread_create(&writers[i], NULL, writer, writerID);
    }

    pthread_join(readers[0], NULL);

    free(readers);
    free(writers);

    return 0;
}

