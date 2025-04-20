#include <stdio.h>
#include "archiver.h"

int main(int argc, char **argv) {

    for (int i = 0; i < argc; i++) {
        argv[i] = argv[i + 1];
    }

    FILE *archiver = fopen("teste.vc", "r+b");
    // option_ip(archiver, argv, argc - 1);
    // option_c(archiver);
    char *files_to_extract[] = { "teste3.txt", "teste4.txt" };
    option_x(archiver, files_to_extract, 2);

    option_m(archiver, "teste3.txt", "teste1.txt");
    // option_c(archiver);
    //
    // // option_c(archiver);
    // char *files_to_remove[] = { "teste3.txt", "teste4.txt" };
    // option_r(archiver, files_to_remove, 2);
    // option_c(archiver);

    fclose(archiver);

    return 0;
}