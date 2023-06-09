#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "Functions.h"

unsigned int readingPersons;
unsigned int writingPersons;
unsigned int waitingWriters;

pthread_mutex_t mutex;
pthread_cond_t canRead, canWrite;

#define randomTime() ((rand() & ((1 << 16) - 1)) + 10000) // <10000, 10000 + 2^16 - 1>

void *reader(void *a)
{
    srand(time(NULL));
    while (1)
    {
        pthread_mutex_lock(&mutex);

        while (writingPersons > 0 || waitingWriters > 0)
        {
            pthread_cond_wait(&canRead, &mutex);
        }

        readingPersons++;
        info();

        pthread_mutex_unlock(&mutex);

        usleep(randomTime());

        pthread_mutex_lock(&mutex);

        readingPersons--;
        info();

        if (readingPersons == 0 && waitingWriters > 0)
        {
            pthread_cond_signal(&canWrite);
        }

        pthread_mutex_unlock(&mutex);
        usleep(randomTime());
    }
    return NULL;
}

void *writer(void *a)
{
    srand(time(NULL));
    while (1)
    {
        pthread_mutex_lock(&mutex);

        waitingWriters++;

        while (writingPersons > 0 || readingPersons > 0)
            pthread_cond_wait(&canWrite, &mutex);

        waitingWriters--;
        writingPersons++;
        info();

        pthread_mutex_unlock(&mutex);

        usleep(randomTime());

        pthread_mutex_lock(&mutex);

        writingPersons--;
        info();

        if (waitingWriters > 0)
        {
            pthread_cond_signal(&canWrite);
        }
        else
        {
            pthread_cond_broadcast(&canRead);
        }

        pthread_mutex_unlock(&mutex);
        usleep(randomTime());
    }
    return NULL;
}

int main(int argc, char **argv)
{
    if (options(argc, argv))
    {
        printf("./[PROGRAM] -R [LB CZYTELNIKOW] -W [LB PISARZY]\n");
        return -1;
    }

    readersCount = 10;
    writersCount = 4;

    writingPersons = 0;
    waitingWriters = 0;
    readingPersons = 0;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&canWrite, NULL);
    pthread_cond_init(&canRead, NULL);
    pthread_t writers[writersCount];
    pthread_t readers[readersCount];

    for (int i = 0; i < readersCount; ++i)
        pthread_create(&readers[i], NULL, reader, NULL);
    sleep(2);
    for (int i = 0; i < writersCount; ++i)
        pthread_create(&writers[i], NULL, writer, NULL);
    for (int i = 0; i < readersCount; ++i)
        pthread_join(readers[i], NULL);
    for (int i = 0; i < writersCount; ++i)
        pthread_join(writers[i], NULL);
    return 0;
}