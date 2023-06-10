#include "Functions.h"

pthread_mutex_t readerMutex;
pthread_mutex_t writerMutex;

void *reader(void *arg)
{
    srand(time(NULL));
    while (1)
    {
        // Poczatek Czytania
        pthread_mutex_lock(&readerMutex);
        if (readingPersons == 0)
            pthread_mutex_lock(&writerMutex);
        readingPersons++;
        info();
        pthread_mutex_unlock(&readerMutex);

        // Czas Czytania
        usleep(randomTime());

        // Koniec Czytania
        pthread_mutex_lock(&readerMutex);
        readingPersons--;
        info();
        if (readingPersons == 0)
            pthread_mutex_unlock(&writerMutex);
        pthread_mutex_unlock(&readerMutex);

        // Wyjscie z czytelni
        // Dojscie do kolejki
        usleep(randomTime());
    }
    return NULL;
}

void *writer(void *arg)
{
    srand(time(NULL));
    while (1)
    {
        // Poczatek Pisania
        pthread_mutex_lock(&writerMutex);
        writingPersons++;
        info();

        // Czas pisania
        usleep(randomTime());

        // Koniec Pisania
        writingPersons--;
        info();
        pthread_mutex_unlock(&writerMutex);

        // Wyjscie z czytelni
        // Dojscie do kolejki
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

    pthread_mutex_init(&readerMutex, NULL);
    pthread_mutex_init(&writerMutex, NULL);

    pthread_t writingPersons[writersCount];
    pthread_t readingPersons[readersCount];

    for (int i = 0; i < writersCount; i++)
        pthread_create(&writingPersons[i], NULL, writer, NULL);

    for (int i = 0; i < readersCount; i++)
        pthread_create(&readingPersons[i], NULL, reader, NULL);

    for (int i = 0; i < writersCount; i++)
        pthread_join(writingPersons[i], NULL);

    for (int i = 0; i < readersCount; i++)
        pthread_join(readingPersons[i], NULL);

    return 0;
}
