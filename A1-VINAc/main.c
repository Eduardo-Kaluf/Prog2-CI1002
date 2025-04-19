#include <stdio.h>
#include "archiver.h"

int main(int argc, char **argv) {

    for (int i = 0; i < argc; i++) {
        argv[i] = argv[i + 1];
    }

    // FILE *archiver = fopen("teste.vc", "r+b");
    //
    // option_ip(archiver, argv, argc - 1);

    // archiver = fopen("teste.vc", "r+b");
    //
    // option_c(archiver);
    //
    FILE *archiver = fopen("teste.vc", "r+b");

    option_x(archiver);

    return 0;
}