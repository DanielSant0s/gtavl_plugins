#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 256

int main() {
    FILE *f;
    f = fopen("symbols.dat", "r");

    if (f == NULL) {
        perror("Error while open symbols.dat");
        return EXIT_FAILURE;
    }

    char linha[MAX_LINE_LENGTH];
    char funcName[256];
    unsigned int endereco;

    while (fgets(linha, sizeof(linha), f) != NULL) {
        if (linha[0] == '#' || linha[0] == '\n' || linha[0] == '\r') {
            continue;
        }

        if (sscanf(linha, "%255s %x", funcName, &endereco) == 2) {
            printf("%s 0x%X\n", funcName, endereco);
        } else {
            fprintf(stderr, "Erro ao ler a linha: %s\n", linha);
        }
    }

    fclose(f);

    return EXIT_SUCCESS;
}