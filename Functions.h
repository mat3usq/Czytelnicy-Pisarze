#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

unsigned int readersCount = 10;
unsigned int writersCount = 10;
unsigned int readingPersons = 0;
unsigned int writingPersons = 0;

int options(int N, char **argv)
{
    if (N > 5)
        return 1;

    for (int i = 0; i < N; i++)
        if (strcmp(argv[i], "-R") == 0)
            readersCount = atoi(argv[i + 1]);
        else if (strcmp(argv[i], "-W") == 0)
            writersCount = atoi(argv[i + 1]);

    return 0;
}

void info()
{
    printf("ReaderQ: %i WriterQ: %i [in: R: %i W: %i]\n", readersCount - readingPersons, writersCount - writingPersons, readingPersons, writingPersons);
}

int randomTime()
{
    int randomNumber = rand() % 900001 + 100000; // Losowanie liczby z zakresu 0-900000 i dodanie 100000
    return randomNumber;
}
