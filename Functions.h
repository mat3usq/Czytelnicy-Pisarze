#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

unsigned int readersCount = 10; // Liczba czytelników
unsigned int writersCount = 10; // Liczba pisarzy
unsigned int readingPersons = 0; // Liczba obecnie czytających osób
unsigned int writingPersons = 0; // Liczba obecnie piszących osób

/* Funkcja analizujaca opcje */
int options(int N, char **argv)
{
    if (N > 5)
        return 1;

    for (int i = 0; i < N; i++)
        if (strcmp(argv[i], "-R") == 0)
            readersCount = atoi(argv[i + 1]); // Ustawianie liczby czytelników na podstawie argumentu -R
        else if (strcmp(argv[i], "-W") == 0)
            writersCount = atoi(argv[i + 1]); // Ustawianie liczby pisarzy na podstawie argumentu -W

    return 0;
}

/* Funkcja wyświetlająca informacje */
void info()
{
    printf("ReaderQ: %i WriterQ: %i [in: R: %i W: %i]\n", readersCount - readingPersons, writersCount - writingPersons, readingPersons, writingPersons);
}

/* Funkcja generująca losowy czas */
int randomTime()
{
    int randomNumber = rand() % 900001 + 100000; // Losowanie liczby z zakresu 0-900000 i dodanie 100000
    return randomNumber;
}
