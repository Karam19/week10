#include <dirent.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define FOLDER \
  "/mnt/c/Users/79871/desktop/OS/week10/tmp/"

#define N 100
#define SIZE 256
size_t n = 0;

typedef struct {
    char *name;
    ino_t inode;
    int printed;
} pair_t;

pair_t *pairs;

int main(int argc, char const *argv[]) {
    pairs = (pair_t *)calloc(N, sizeof(pair_t));

    DIR *dirp = opendir(FOLDER);
    if (dirp == NULL) return 1;
    size_t len = strlen(FOLDER);

    struct dirent *d;

    while ((d = readdir(dirp)) != NULL) {
        (pairs + n)->name = (char *)calloc(SIZE, sizeof(char));
        strcpy((pairs + n)->name, d->d_name);
        (pairs + n)->inode = d->d_ino;
        (pairs + n)->printed = 0;
        n++;
    }
    closedir(dirp);

    for (size_t i = 0; i < n; i++) {
        if ((pairs + i)->printed) continue;

        struct stat b;
        char *path =(char *)malloc(strlen(FOLDER) + strlen((pairs + i)->name) + 1);
        strcpy(path, FOLDER);
        strcat(path, (pairs + i)->name);

        stat(path, &b);
        free(path);
        if (b.st_nlink < 2) continue;

        printf("i-node %ju: {\n", (uintmax_t)(pairs + i)->inode);

        for (size_t j = 0; j < n; ++j) {
            if ((pairs + j)->inode == (pairs + i)->inode && (!(pairs + j)->printed)) {
                printf("  '%s',\n", (pairs + j)->name);
                (pairs + j)->printed = 1;
            }
        }
        printf("}\n");
    }
    return 0;
}