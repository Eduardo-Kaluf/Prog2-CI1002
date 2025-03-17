#include <stdio.h>

// Notas

// Podemos compor um tipo de dado utilizando outros, pois um tipo de dado é somente como o compilador interpreta a memória:
// Exemplo:

void exemplo1() {
    char v[2];
    short *p;
    p = (short *) v;
    *p = 15000;
}


// Aqui usamos p como um short apesar dele estar apontando para um vetor de chars
// Podemos fazer o contrário tambem.

void exemplo2() {
    int x;
    char *v;
    v = (char *) &x;
    v[0] = 'a';
    v[1] = 'n';
    v[2] = 'a';
    v[3] = '/0';

    printf("%d", x);
}

int main() {
    exemplo1();
    exemplo2();

    return 0;
}

// Diferenças entre float e inteiros

// Inteiro -> 31 bits indicam o valor; 1 bit que indica o sinal

// float -> 23 bits indicam o valor; 8 bits indicam o expoente; 1 bit indica o sinal (IEEE 754)

// BIG X LITTLE ENDIAN

// Normalmente usamos little endian, onde os valores mais significativos ficam no começo do byte
// No big endian os BYTES são espelhados, ou seja, os bytes de maior valor significativo ficam no final (ver imagem)
// A rede normalmente utiliza o método big endian
