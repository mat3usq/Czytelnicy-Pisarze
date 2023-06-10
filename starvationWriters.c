#include "Functions.h"

pthread_mutex_t readerMutex;    // Mutex dla czytelników
pthread_mutex_t writerMutex;    // Mutex dla pisarzy

void *reader(void *arg)
{
    srand(time(NULL));           // Inicjalizacja generatora liczb losowych
    while (1)
    {
        // Początek Czytania
        pthread_mutex_lock(&readerMutex);      // Zablokowanie mutexa dla czytelników

        if (readingPersons == 0)
            pthread_mutex_lock(&writerMutex);  // Jeśli nie ma żadnych czytelników, blokuj mutex dla pisarzy

        readingPersons++;                       // Zwiększenie liczby czytelników
        info();

        pthread_mutex_unlock(&readerMutex);    // Odblokowanie mutexa dla czytelników

        // Czas Czytania
        usleep(randomTime());

        // Koniec Czytania
        pthread_mutex_lock(&readerMutex);       // Zablokowanie mutexa dla czytelników

        readingPersons--;                       // Zmniejszenie liczby czytelników
        info();

        if (readingPersons == 0)
            pthread_mutex_unlock(&writerMutex); // Jeśli nie ma żadnych czytelników, odblokuj mutex dla pisarzy

        pthread_mutex_unlock(&readerMutex);     // Odblokowanie mutexa dla czytelników

        // Wyjście z czytelni
        // Dojście do kolejki
        usleep(randomTime());
    }
    return NULL;
}

void *writer(void *arg)
{
    srand(time(NULL));           // Inicjalizacja generatora liczb losowych
    while (1)
    {
        // Początek Pisania
        pthread_mutex_lock(&writerMutex);      // Zablokowanie mutexa dla pisarzy

        writingPersons++;                       // Zwiększenie liczby pisarzy
        info();

        // Czas pisania
        usleep(randomTime());

        // Koniec Pisania
        writingPersons--;                       // Zmniejszenie liczby pisarzy
        info();

        pthread_mutex_unlock(&writerMutex);    // Odblokowanie mutexa dla pisarzy

        // Wyjście z czytelni
        // Dojście do kolejki
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

    pthread_mutex_init(&readerMutex, NULL);     // Inicjalizacja mutexa dla czytelników
    pthread_mutex_init(&writerMutex, NULL);     // Inicjalizacja mutexa dla pisarzy

    pthread_t writingPersons[writersCount];     // Tablica wątków pisarzy
    pthread_t readingPersons[readersCount];     // Tablica wątków czytelników

    for (int i = 0; i < writersCount; i++)
        pthread_create(&writingPersons[i], NULL, writer, NULL);     // Tworzenie wątków pisarzy

    for (int i = 0; i < readersCount; i++)
        pthread_create(&readingPersons[i], NULL, reader, NULL);     // Tworzenie wątków czytelników

    for (int i = 0; i < writersCount; i++)
        pthread_join(writingPersons[i], NULL);                      // Oczekiwanie na zakończenie wątków pisarzy

    for (int i = 0; i < readersCount; i++)
        pthread_join(readingPersons[i], NULL);                      // Oczekiwanie na zakończenie wątków czytelników

    return 0;
}
