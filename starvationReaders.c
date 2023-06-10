#include "Functions.h"

unsigned int waitingWriters;        // Liczba oczekujących pisarzy

pthread_mutex_t mutex;              // Mutex dla synchronizacji dostępu do zasobów
pthread_cond_t canRead, canWrite;   // Warunki oczekiwania dla czytelników i pisarzy

void *reader(void *arg)
{
    srand(time(NULL));              // Inicjalizacja generatora liczb losowych
    while (1)
    {
        // Początek Czytania
        pthread_mutex_lock(&mutex);  // Zablokowanie mutexa

        while (writingPersons > 0 || waitingWriters > 0)
        {
            pthread_cond_wait(&canRead, &mutex);    // Oczekiwanie na warunek czytania
        }

        readingPersons++;             // Zwiększenie liczby czytelników
        info();

        pthread_mutex_unlock(&mutex); // Odblokowanie mutexa

        // Czas Czytania
        usleep(randomTime());

        // Koniec Czytania
        pthread_mutex_lock(&mutex);   // Zablokowanie mutexa

        readingPersons--;             // Zmniejszenie liczby czytelników
        info();

        if (readingPersons == 0 && waitingWriters > 0)
        {
            pthread_cond_signal(&canWrite);         // Jeśli nie ma żadnych czytelników i są oczekujący pisarze, sygnalizuj pisarzowi, że może pisać
        }

        pthread_mutex_unlock(&mutex); // Odblokowanie mutexa

        // Wyjście z czytelni
        // Dojście do kolejki
        usleep(randomTime());
    }
    return NULL;
}

void *writer(void *arg)
{
    srand(time(NULL));              // Inicjalizacja generatora liczb losowych
    while (1)
    {
        // Początek Pisania
        pthread_mutex_lock(&mutex);  // Zablokowanie mutexa

        waitingWriters++;             // Zwiększenie liczby oczekujących pisarzy

        while (writingPersons > 0 || readingPersons > 0)
        {
            pthread_cond_wait(&canWrite, &mutex);   // Oczekiwanie na warunek pisania
        }

        waitingWriters--;             // Zmniejszenie liczby oczekujących pisarzy
        writingPersons++;             // Zwiększenie liczby pisarzy
        info();

        pthread_mutex_unlock(&mutex); // Odblokowanie mutexa

        // Czas pisania
        usleep(randomTime());

        // Koniec Pisania
        pthread_mutex_lock(&mutex);   // Zablokowanie mutexa

        writingPersons--;             // Zmniejszenie liczby pisarzy
        info();

        if (waitingWriters > 0)
        {
            pthread_cond_signal(&canWrite);         // Jeśli są oczekujący pisarze, sygnalizuj pisarzowi, że może pisać
        }
        else
        {
            pthread_cond_broadcast(&canRead);       // W przeciwnym razie sygnalizuj wszystkim oczekującym czytelnikom, że mogą czytać
        }

        pthread_mutex_unlock(&mutex); // Odblokowanie mutexa

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

    pthread_mutex_init(&mutex, NULL);               // Inicjalizacja mutexa
    pthread_cond_init(&canWrite, NULL);             // Inicjalizacja warunku oczekiwania dla pisarzy
    pthread_cond_init(&canRead, NULL);              // Inicjalizacja warunku oczekiwania dla czytelników

    pthread_t writers[writersCount];                // Tablica wątków pisarzy
    pthread_t readers[readersCount];                // Tablica wątków czytelników

    for (int i = 0; i < readersCount; i++)
        pthread_create(&readers[i], NULL, reader, NULL);     // Tworzenie wątków czytelników

    for (int i = 0; i < writersCount; i++)
        pthread_create(&writers[i], NULL, writer, NULL);     // Tworzenie wątków pisarzy

    for (int i = 0; i < readersCount; i++)
        pthread_join(readers[i], NULL);                      // Oczekiwanie na zakończenie wątków czytelników

    for (int i = 0; i < writersCount; i++)
        pthread_join(writers[i], NULL);                      // Oczekiwanie na zakończenie wątków pisarzy

    return 0;
}
