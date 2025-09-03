#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>

typedef struct dirent dirent_t;

int main() {
    DIR *dirp = opendir(".");
    if(dirp == NULL) {
        perror("opendir");
        return EXIT_FAILURE;
    }
    dirent_t *entry;

    while((entry = readdir(dirp)) != NULL) {
        printf("%s \n", entry->d_name);
    }

    if(closedir(dirp) != 0) {
        perror("closedir");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}