#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct sala {
    char nome[50];
    struct sala *esq;
    struct sala *dir;
} Sala;

Sala* criarSala(char nome[]) {
    Sala* nova = (Sala*)malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    nova->esq = NULL;
    nova->dir = NULL;
    return nova;
}

void explorarSalas(Sala* atual) {
    char escolha;
    while (atual != NULL) {
        printf("\nVoce esta em: %s\n", atual->nome);
        if (atual->esq == NULL && atual->dir == NULL) {
            printf("Nao ha mais caminhos. Fim da exploracao.\n");
            break;
        }
        printf("Ir para esquerda (e), direita (d) ou sair (s): ");
        scanf(" %c", &escolha);
        if (escolha == 'e' && atual->esq != NULL) {
            atual = atual->esq;
        } else if (escolha == 'd' && atual->dir != NULL) {
            atual = atual->dir;
        } else if (escolha == 's') {
            printf("Saindo da exploracao.\n");
            break;
        } else {
            printf("Caminho invalido.\n");
        }
    }
}

int main() {
    Sala* hall = criarSala("Hall de entrada");
    Sala* salaEstar = criarSala("Sala de estar");
    Sala* cozinha = criarSala("Cozinha");
    Sala* biblioteca = criarSala("Biblioteca");
    Sala* jardim = criarSala("Jardim");
    Sala* escritorio = criarSala("Escritorio");
    Sala* porao = criarSala("Porao");

    hall->esq = salaEstar;
    hall->dir = cozinha;
    salaEstar->esq = biblioteca;
    salaEstar->dir = jardim;
    cozinha->esq = escritorio;
    cozinha->dir = porao;

    explorarSalas(hall);

    free(hall);
    free(salaEstar);
    free(cozinha);
    free(biblioteca);
    free(jardim);
    free(escritorio);
    free(porao);

    return 0;
}
