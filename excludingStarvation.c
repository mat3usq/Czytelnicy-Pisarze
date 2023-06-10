#include "Functions.h"

pthread_mutex_t mutex;               // Mutex dla synchronizacji dostępu do zasobów
pthread_cond_t canWrite;             // Warunek oczekiwania dla pisarzy
pthread_cond_t canRead;              // Warunek oczekiwania dla czytelników
unsigned int waitingReaders = 0;     // Liczba oczekujących czytelników
unsigned int waitingWriters = 0;     // Liczba oczekujących pisarzy

void *reader(void *arg)
{
    while (1)
    {
        // Poczatek Czytania
        pthread_mutex_lock(&mutex);    // Zablokowanie mutexa

        if (waitingWriters > 0 || writingPersons > 0)
        {
            waitingReaders++;          // Zwiększenie liczby oczekujących czytelników
            pthread_cond_wait(&canRead, &mutex);   // Oczekiwanie na warunek czytania
        }
        else
        {
            readingPersons++;           // Zwiększenie liczby czytelników
            info();
        }

        pthread_mutex_unlock(&mutex);   // Odblokowanie mutexa

        // Czas Czytania
        usleep(randomTime());

        // Koniec Czytania
        pthread_mutex_lock(&mutex);     // Zablokowanie mutexa
        readingPersons--;               // Zmniejszenie liczby czytelników

        if (readingPersons == 0)
            pthread_cond_signal(&canWrite);     // Jeśli nie ma żadnych czytelników, sygnalizuj pisarzowi, że może pisać

        pthread_mutex_unlock(&mutex);   // Odblokowanie mutexa

        // Wyjscie z czytelni
        // Dojscie do kolejki
        usleep(randomTime());
    }
}

void *writer(void *arg)
{
    while (1)
    {
        // Poczatek Pisania
        pthread_mutex_lock(&mutex);     // Zablokowanie mutexa

        if (readingPersons > 0 || writingPersons > 0)
        {
            waitingWriters++;           // Zwiększenie liczby oczekujących pisarzy
            pthread_cond_wait(&canWrite, &mutex);   // Oczekiwanie na warunek pisania
            waitingWriters--;
        }

        writingPersons = 1;             // Ustawienie liczby pisarzy na 1 (aktualnie piszący)
        info();
        pthread_mutex_unlock(&mutex);   // Odblokowanie mutexa

        // Czas pisania
        usleep(randomTime());

        // Koniec Pisania
        pthread_mutex_lock(&mutex);     // Zablokowanie mutexa
        writingPersons = 0;             // Zerowanie liczby pisarzy (żaden pisarz nie pisze)

        if (waitingReaders == 0)
            pthread_cond_signal(&canWrite);     // Jeśli nie ma oczekujących czytelników, sygnalizuj pisarzowi, że może pisać
        else
        {
            pthread_cond_broadcast(&canRead);   // W przeciwnym razie sygnalizuj wszystkim oczekującym czytelnikom, że mogą czytać
            readingPersons += waitingReaders;   // Zwiększenie liczby czytelników o liczbę oczekujących czytelników
            waitingReaders = 0;                  // Wyzerowanie liczby oczekujących czytelników
            info();
        }

        pthread_mutex_unlock(&mutex);   // Odblokowanie mutexa

        // Wyjscie z czytelni
        // Dojscie do kolejki
        usleep(randomTime());
    }
}

int main(int argc, char *argv[])
{
    if (options(argc, argv))
    {
        printf("./[PROGRAM] -R [LB CZYTELNIKOW] -W [LB PISARZY]\n");
        return -1;
    }

    pthread_mutex_init(&mutex, NULL);             // Inicjalizacja mutexa
    pthread_cond_init(&canWrite, NULL);           // Inicjalizacja warunku oczekiwania dla pisarzy
    pthread_cond_init(&canRead, NULL);            // Inicjalizacja warunku oczekiwania dla czytelników

    pthread_t writers[writersCount];              // Tablica wątków pisarzy
    pthread_t readers[readersCount];              // Tablica wątków czytelników

    for (int i = 0; i < writersCount; i++)
        pthread_create(&writers[i], NULL, writer, NULL);     // Tworzenie wątków pisarzy

    for (int i = 0; i < readersCount; i++)
        pthread_create(&readers[i], NULL, reader, NULL);     // Tworzenie wątków czytelników

    for (int i = 0; i < writersCount; i++)
        pthread_join(writers[i], NULL);                      // Oczekiwanie na zakończenie wątków pisarzy

    for (int i = 0; i < readersCount; i++)
        pthread_join(readers[i], NULL);                      // Oczekiwanie na zakończenie wątków czytelników

    return 0;
}
