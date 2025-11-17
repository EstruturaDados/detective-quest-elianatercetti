#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct sala {
    char nome[50];
    char pista[50];
    struct sala *esq;
    struct sala *dir;
} Sala;

typedef struct pistaNode {
    char pista[50];
    struct pistaNode *esq;
    struct pistaNode *dir;
} PistaNode;

Sala* criarSala(char nome[], char pista[]) {
    Sala* nova = (Sala*)malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);
    nova->esq = NULL;
    nova->dir = NULL;
    return nova;
}

PistaNode* criarPistaNode(char pista[]) {
    PistaNode* nova = (PistaNode*)malloc(sizeof(PistaNode));
    strcpy(nova->pista, pista);
    nova->esq = NULL;
    nova->dir = NULL;
    return nova;
}

PistaNode* inserirPista(PistaNode* raiz, char pista[]) {
    if (strlen(pista) == 0) return raiz;
    if (raiz == NULL) return criarPistaNode(pista);
    if (strcmp(pista, raiz->pista) < 0)
        raiz->esq = inserirPista(raiz->esq, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->dir = inserirPista(raiz->dir, pista);
    return raiz;
}

void exibirPistas(PistaNode* raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esq);
        printf("- %s\n", raiz->pista);
        exibirPistas(raiz->dir);
    }
}

void explorarSalasComPistas(Sala* atual, PistaNode** arvorePistas) {
    char escolha;
    while (atual != NULL) {
        printf("\nVoce esta em: %s\n", atual->nome);
        if (strlen(atual->pista) > 0) {
            printf("Voce encontrou uma pista: %s\n", atual->pista);
            *arvorePistas = inserirPista(*arvorePistas, atual->pista);
        }
        printf("Ir para esquerda (e), direita (d) ou sair (s): ");
        scanf(" %c", &escolha);
        if (escolha == 'e' && atual->esq != NULL)
            atual = atual->esq;
        else if (escolha == 'd' && atual->dir != NULL)
            atual = atual->dir;
        else if (escolha == 's') {
            printf("\nExploracao encerrada.\n");
            break;
        } else
            printf("Caminho invalido.\n");
    }
}

int main() {
    Sala* hall = criarSala("Hall de Entrada", "Chave enferrujada");
    Sala* salaEstar = criarSala("Sala de Estar", "");
    Sala* cozinha = criarSala("Cozinha", "Papel queimado");
    Sala* biblioteca = criarSala("Biblioteca", "Livro rasgado");
    Sala* jardim = criarSala("Jardim", "");
    Sala* escritorio = criarSala("Escritorio", "Envelope misterioso");
    Sala* porao = criarSala("Porao", "Mancha estranha no chao");

    hall->esq = salaEstar;
    hall->dir = cozinha;
    salaEstar->esq = biblioteca;
    salaEstar->dir = jardim;
    cozinha->esq = escritorio;
    cozinha->dir = porao;

    PistaNode* arvorePistas = NULL;

    explorarSalasComPistas(hall, &arvorePistas);

    printf("\nPistas coletadas em ordem alfabetica:\n");
    exibirPistas(arvorePistas);

    return 0;
}
