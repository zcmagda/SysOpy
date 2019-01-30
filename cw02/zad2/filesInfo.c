// Zadanie 2
// Operacje na strukturze katalogów (45%)
//
// Napisz program wyszukujący w drzewie katalogu (ścieżka do katalogu jest pierwszym argumentem programu), w zależności od wartości drugiego argumentu ('<', '>','=') , pliki zwykłe z datą modyfikacji wcześniejszą, późniejszą lub równą dacie podanej jako trzeci argument programu.
// Program ma wypisać na standardowe wyjście następujące informacje znalezionych plików:
//     ścieżka bezwzględna pliku
//     rozmiar w bajtach
//     prawa dostępu do pliku (w formacie używanym przez ls -l, np. r--r--rw- oznacza prawa odczytu dla wszystkich i zapisu dla właściciela)
//     datę ostatniej modyfikacji
//
// Ścieżka podana jako argument wywołania może być względna lub bezwzględna.
// Program powinien wyszukiwać tylko pliki zwyczajne (bez dowiązań, urządzeń blokowych, itp).
// Program nie powinien podążać za dowiązaniami symbolicznymi do katalogów.
//
// Program należy zaimplementować w dwóch wariantach:
//     Korzystając z funkcji opendir, readdir oraz funkcji z rodziny stat (25%)
//     Korzystając z funkcji nftw (20%)
//
// W ramach testowania funkcji utwórz w badanej strukturze katalogów jakieś dowiązania symboliczne.

#define __USE_XOPEN
#define _XOPEN_SOURCE
#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ftw.h>
#include <math.h>
#include <dirent.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

struct parsedArgs {
    char* path;
    char symbol;
    char* date;
};

struct parsedArgs tmpStruct;
#define FOO tmpStruct

char* getPermissions(const struct stat* buffer) {

        char* permissions = (char*) malloc(sizeof(char) * 11);

        permissions[0] = S_ISDIR(buffer -> st_mode) ? 'd' : '-';
        permissions[1] = (buffer -> st_mode & S_IRUSR) ? 'r' : '-';
        permissions[2] = (buffer -> st_mode & S_IWUSR) ? 'w' : '-';
        permissions[3] = (buffer -> st_mode & S_IXUSR) ? 'x' : '-';
        permissions[4] = (buffer -> st_mode & S_IRGRP) ? 'r' : '-';
        permissions[5] = (buffer -> st_mode & S_IWGRP) ? 'w' : '-';
        permissions[6] = (buffer -> st_mode & S_IXGRP) ? 'x' : '-';
        permissions[7] = (buffer -> st_mode & S_IROTH) ? 'r' : '-';
        permissions[8] = (buffer -> st_mode & S_IWOTH) ? 'w' : '-';
        permissions[9] = (buffer -> st_mode & S_IXOTH) ? 'x' : '-';
        permissions[10] = '\0';

        return permissions;
}

void printFileInfo (const struct stat* buffer, const char* path, const char *name) {

        char* permissions =  (char*) getPermissions(buffer);

        printf("%-20s%-100s%-12lld%-8s      %-16s",
        name,
        path,
        (long long int) buffer->st_size,
        permissions,
        ctime(&(buffer->st_mtime))
        );

        free(permissions);
}

int compareDate (char* date, char symbol, time_t fileTime) {

        struct tm* parsedDate = (struct tm*) malloc(sizeof(struct tm));

        if (strptime(date, "%Y-%m-%d %H:%M:%S", parsedDate) == NULL)
                exit(EXIT_FAILURE);

        time_t parsedTime = mktime(parsedDate);

        if (symbol == '=')
                return fabs(difftime(parsedTime, fileTime)) < 0.1 ? 1 : 0;
        else if (symbol == '>')
                return difftime(parsedTime, fileTime) > 0 ? 1 : 0;
        else if (symbol == '<')
                return difftime(parsedTime, fileTime) < 0 ? 1 : 0;
        else
                exit(EXIT_FAILURE);
}

void wayA(const char * path_name, char symbol,  char *date) {

        struct stat sb;
        struct dirent * currentDir;
        DIR * path;

        if ((path = opendir(path_name))) {
                while ((currentDir = readdir(path))) {
                        char currentDirPath[1024];
                        snprintf(currentDirPath, sizeof(currentDirPath), "%s/%s", path_name, currentDir->d_name);

                        if (currentDir->d_type == DT_DIR  && (!S_ISLNK(sb.st_mode))) {
                                if (strcmp(currentDir->d_name, ".") != 0 && strcmp(currentDir->d_name, "..") != 0) {
                                        wayA(currentDirPath, symbol, date);
                                }
                        }
                        else {
                                if (currentDir->d_type == DT_REG && (stat(path_name, &sb)  != -1) && compareDate(date, symbol, sb.st_mtime)) {
                                                printFileInfo(&sb, currentDirPath, currentDir->d_name);
                                }
                        }
                }

                closedir(path);
        }
        else {
                printf("Can't open the path \n");

                exit(EXIT_FAILURE);
        }
}

int fn (const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf) {

        if (typeflag == FTW_F && typeflag != FTW_SL && compareDate(FOO.date, FOO.symbol, sb -> st_mtime)){
                const char *name = fpath + ftwbuf->base;
                printFileInfo(sb, fpath, name);
        }

        return 0;
}

struct parsedArgs parseArgs  (int argc, char* argv[]) {
        if (argc < 3) {
                printf("Ilegal arguments number\n");

                exit(EXIT_FAILURE);
        }

        if (!(*argv[2] == '=' || *argv[2] == '>' || *argv[2] == '<')) {
                printf("Wrong second argument \n");

                exit(EXIT_FAILURE);
        }

        struct parsedArgs args;

        args.path = argv[1];
        args.symbol = *argv[2];
        args.date = argv[3];

        return args;
}

void wayB (int argc, char **argv) {

        FOO = parseArgs(argc, argv);

        if (nftw(FOO.path, fn, 1024, FTW_PHYS | FTW_NS) == -1) {
                perror("nftw");
                exit(EXIT_FAILURE);
        }
}
int main (int argc, char **argv) {

        struct parsedArgs args = parseArgs(argc, argv);

        printf("%-20s%-100s%-12s%-8s      %-16s",
        "\nName",
        "Path",
        "Size",
        "Permissions  ",
        "Last file modification\n"
        );

        printf("\nWay A\n");
        wayA(args.path, args.symbol, args.date);
        printf("\nWay B\n");
        wayB(argc, argv);
        printf("\n");

        return 0;
}
