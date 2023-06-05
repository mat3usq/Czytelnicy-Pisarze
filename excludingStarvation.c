#include "Functions.h"

pthread_mutex_t mutex;
pthread_cond_t canWrite;
pthread_cond_t canRead;
unsigned int waitingReaders = 0;
unsigned int waitingWriters = 0;

void startReading()
{
    pthread_mutex_lock(&mutex);
    if (waitingWriters > 0 || writingPersons > 0)
    {
        waitingReaders++;
        pthread_cond_wait(&canRead, &mutex);
    }
    else
    {
        readingPersons++;
        info();
    }
    pthread_mutex_unlock(&mutex);
}

void stopReading()
{
    pthread_mutex_lock(&mutex);
    readingPersons--;

    if (readingPersons == 0)
        pthread_cond_signal(&canWrite);

    pthread_mutex_unlock(&mutex);
}

void startWriting()
{
    pthread_mutex_lock(&mutex);

    if (readingPersons > 0 || writingPersons > 0)
    {
        waitingWriters++;
        pthread_cond_wait(&canWrite, &mutex);
        waitingWriters--;
    }

    writingPersons = 1;
    info();
    pthread_mutex_unlock(&mutex);
}

void stopWriting()
{
    pthread_mutex_lock(&mutex);
    writingPersons = 0;

    if (waitingReaders == 0)
        pthread_cond_signal(&canWrite);
    else
    {
        pthread_cond_broadcast(&canRead);
        readingPersons += waitingReaders;
        waitingReaders = 0;
        info();
    }

    pthread_mutex_unlock(&mutex);
}

void *reader(void *arg)
{
    while (1)
    {
        startReading();
        usleep(randomNumber());
        stopReading();
        usleep(randomNumber());
    }
}

void *writer(void *arg)
{
    while (1)
    {
        startWriting();
        usleep(randomNumber());
        stopWriting();
        usleep(randomNumber());
    }
}

int main(int argc, char *argv[])
{
    printf("./[PROGRAM] [-R <liczba czytelnikow>] [-W <liczba pisarzy>]\n");
    options(argc, argv);

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&canWrite, NULL);
    pthread_cond_init(&canRead, NULL);

    pthread_t writers[writersCount];
    pthread_t readers[readersCount];

    for (int i = 0; i < writersCount; i++)
        pthread_create(&writers[i], NULL, writer, NULL);
    for (int i = 0; i < readersCount; i++)
        pthread_create(&readers[i], NULL, reader, NULL);
    for (int i = 0; i < writersCount; i++)
        pthread_join(writers[i], NULL);
    for (int i = 0; i < readersCount; i++)
        pthread_join(readers[i], NULL);
}