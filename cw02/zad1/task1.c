// Zadanie 1
// Porównanie wydajności systemowych i bibliotecznych funkcji we/wy (55%)
//
// (30%) Celem zadania jest napisanie programu porównującego wydajność systemowych i bibliotecznych funkcji wejścia/wyjścia. Program operował będzie na przechowywanej w pliku tablicy rekordów.
// Dla uproszczenia pojedynczy rekord będzie tablicą bajtów o stałej wielkości. Nazwa pliku, wielkość oraz ilość rekordów stanowić będą argumenty wywołania programu.
//
// Program udostępniać powinien operacje:
//         generate - tworzenie pliku z rekordami wypełnionego wygenerowaną losową zawartością (można wykorzystać wirtualny generator /dev/random lub w wersji uproszczonej funkcję rand)
//         sort - sortuje rekordy w pliku używając sortowania przez proste wstawianie. Kluczem do sortowania niech będzie wartość pierwszego bajtu rekordu (interpretowanego jako liczba bez znaku - unsigned char).
//                   Podczas sortowania w pamięci powinny być przechowywane jednocześnie najwyżej dwa rekordy (sprowadza się do zamieniania miejscami i porównywania dwóch rekordów).
//         copy - kopiuje plik1 do pliku2. Kopiowanie powinno odbywać się za pomocą bufora o zadanej wielkości rekordu.
//
// Sortowanie i kopiowanie powinno być zaimplementowane w dwóch wariantach:
//         sys - przy użyciu funkcji systemowych: read i write
//         lib - przy użyciu funkcji biblioteki C: fread i fwrite
//
// Rodzaj operacji oraz sposób dostępu do plików ma być wybierany na podstawie argumentu wywołania - np.:
//      ./program generate dane 100 512 powinno losowo generować 100 rekordów o długości 512 bajtów do pliku dane,
//      ./program sort dane 100 512 sys powinien sortować rekordy w pliku dane przy użyciu funkcji systemowych, zakładając że zawiera on 100 rekordów wielkości 512 bajtów
//      ./program copy plik1 plik2 100 512 sys powinno skopiować 100 rekordów pliku 1 do pliku 2 za pomocą funkcji bibliotecznych z wykorzystaniem bufora 512 bajtów
//
// (25%) Dla obu wariantów implementacji przeprowadź pomiary czasu użytkownika i czasu systemowego operacji sortowania i kopiowania. Testy wykonaj dla następujących rozmiarów rekordu: 4, 512, 4096 i 8192 bajty.
// Dla każdego rozmiaru rekordu wykonaj dwa testy różniące się liczbą rekordów w sortowanym pliku. Liczby rekordów dobierz tak, by czas sortowania mieścił się w przedziale od kilku do kilkudziesięciu sekund.
// Porównując dwa warianty implementacji należy korzystać z identycznego pliku do sortowania (po wygenerowaniu, a przed sortowaniem, utwórz jego kopię).
// Zmierzone czasy zestaw w pliku wyniki.txt. Do pliku dodaj komentarz podsumowujący wnioski z testów.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/times.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

void generate(char *filename, int recordsNumber, int recordSize) {
        int file = open(filename, O_CREAT | O_RDWR |  O_EXCL, 0700);

        if (file < 0) {
                file = open(filename, O_RDWR | O_APPEND);
                printf("File already exist, appending \n");
        }

        time_t t;
        srand((unsigned) time(&t));
        char* buffer = (char*) malloc(recordSize * sizeof(char));

        for (int i = 0; i < recordsNumber; i++) {

                for (int j = 0; j < recordSize - 1; j++)
                        buffer[j] =  23 + (rand() % 95);

                buffer[recordSize - 1] = 10; //newline for readability
                write(file, buffer, recordSize);
        }

        close(file);
}

void copy_sys(char *sourceFile, char *destFile, int recordsNumber, int recordSize) {
        int src = open(sourceFile, O_RDONLY);
        int dest = open(destFile, O_WRONLY | O_CREAT | O_APPEND, 0700);
        char* buffer = (char*) malloc(recordSize * sizeof(char));

        if(sourceFile && destFile) {

                for (int i = 0; i < recordsNumber; i++) {
                        read(src, buffer, recordSize);
                        write(dest, buffer, recordSize);
                }
        }
        close(src);
        close(dest);
}

void copy_lib(char *sourceFile, char *destFile, int recordsNumber, int recordSize) {
        FILE *src = fopen(sourceFile, "r");
        FILE *dest = fopen(destFile, "a+");
        char* buffer = (char*) malloc(recordSize * sizeof(char));

        if(sourceFile && destFile) {

                for (int i = 0; i < recordsNumber; i++) {
                        fread(buffer, sizeof(char), (size_t) recordSize, src);
                        fwrite(buffer, sizeof(char), (size_t) recordSize, dest);
                }
        }
        fclose(src);
        fclose(dest);
}

//void sort_sys(char *sourceFile, int recordsNumber, int recordSize) { }

//void sort_lib(char *sourceFile, int recordsNumber, int recordSize) { }

void parseArgs  (int argc, char* argv[]) {

        int recordsNumber, recordSize;

        if ((strcmp(argv[1], "generate") == 0) && (argc == 5)) {
                sscanf(argv[3], "%d", &recordsNumber);
                sscanf(argv[4], "%d", &recordSize);

                generate(argv[2], recordsNumber, recordSize);
        }
        else if ((strcmp(argv[1], "sort") == 0) && (argc == 6)) {
                sscanf(argv[3], "%d", &recordsNumber);
                sscanf(argv[4], "%d", &recordSize);

                if (strcmp(argv[5], "sys") == 0) {}
                        //sort_sys(argv[2],  recordsNumber, recordSize, argv[5]);
                else if (strcmp(argv[5], "lib") == 0) {}
                        //sort_lib(argv[2],  recordsNumber, recordSize, argv[5]);
                else {
                        printf("Illegal arguments \n");
                        exit(EXIT_FAILURE);
                }
        }
        else if ((strcmp(argv[1], "copy") == 0) && (argc == 7)) {
                sscanf(argv[4], "%d", &recordsNumber);
                sscanf(argv[5], "%d", &recordSize);

                if (strcmp(argv[6], "sys") == 0)
                        copy_sys(argv[2], argv[3], recordsNumber, recordSize);
                else if (strcmp(argv[6], "lib") == 0)
                        copy_lib(argv[2], argv[3], recordsNumber, recordSize);
                else {
                        printf("Illegal arguments \n");
                        exit(EXIT_FAILURE);
                }
        }
        else {
            printf("Illegal arguments \n");
            exit(EXIT_FAILURE);
        }
}

int main (int argc, char **argv) {

        struct timespec realTimeStart={0,0}, realTimeEnd={0,0};
        clock_t cpuStart, cpuEnd;
        double realTime, cpuTime;

        cpuStart = clock();
        clock_gettime(CLOCK_REALTIME, &realTimeStart);

        parseArgs(argc, argv);

        clock_gettime(CLOCK_REALTIME, &realTimeEnd);
        cpuEnd = clock();

        //time in seconds
        realTime = ((double)realTimeEnd.tv_sec + 1.0e-9*realTimeEnd.tv_nsec) - ((double)realTimeStart.tv_sec + 1.0e-9*realTimeStart.tv_nsec);
        printf("Real time: %fs\n", realTime);

        cpuTime = ((double) (cpuEnd - cpuStart)) / CLOCKS_PER_SEC;
        printf("CPU time: %fs \n", cpuTime);

        return 0;
}
